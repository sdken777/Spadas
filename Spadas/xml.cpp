
#include "spadas.h"

namespace spadas
{
	const UInt XML_CELL_ARRAY_SEGMENT_SIZE = 256;
	const UInt XML_TEXT_TABLE_SIZE_SMALL = 64;
	const UInt XML_TEXT_TABLE_SIZE_MIDDLE = 2048;
	const UInt XML_TEXT_TABLE_SIZE_LARGE = 65536;

	const String unknownTag = "unknown";
	const String rootTag = "root";

	struct XMLCell
	{
		XMLElement elem;
		XMLCell* root;
		List<XMLCell*> leaves;

		XMLCell() : root(NULL)
		{}
		XMLCell(XMLElement elem) : elem(elem), root(NULL)
		{}
	};

	class XMLVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(XML, Vars)

		ArrayX<XMLCell> cells; // 首个为根节点
		Map<StringSpan, String> textTable;

		XMLVars() : cells(XML_CELL_ARRAY_SEGMENT_SIZE), textTable(XML_TEXT_TABLE_SIZE_SMALL)
		{
			cells.append(XMLElement(rootTag, 0, String()));
		}
		XMLVars(XMLNode rootNode) : cells(XML_CELL_ARRAY_SEGMENT_SIZE), textTable(XML_TEXT_TABLE_SIZE_SMALL)
		{
			XMLCell& rootCell = cells.append(rootNode.value());
			cloneXMLNode(rootNode, rootCell);
		}
		void checkTableSize()
		{
			UInt curSize = textTable.size();
			if (curSize == XML_TEXT_TABLE_SIZE_SMALL)
			{
				Map<StringSpan, String> newTable(XML_TEXT_TABLE_SIZE_MIDDLE);
				for (auto pair = textTable.keyValues().firstElem(); pair.valid(); ++pair)
				{
					newTable[pair->key] = pair->value;
				}
				textTable = newTable;
			}
			else if (curSize == XML_TEXT_TABLE_SIZE_MIDDLE)
			{
				Map<StringSpan, String> newTable(XML_TEXT_TABLE_SIZE_LARGE);
				for (auto pair = textTable.keyValues().firstElem(); pair.valid(); ++pair)
				{
					newTable[pair->key] = pair->value;
				}
				textTable = newTable;
			}
		}
		String touchText(StringSpan& key)
		{
			String output;
			if (textTable.tryGet(key, output)) return output;
			else
			{
				checkTableSize();
				String valString = key.clone();
				textTable[key] = valString;
				return valString;
			}
		}
		String touchText(StringSpan& key, String& val)
		{
			String output;
			if (textTable.tryGet(key, output)) return output;
			else
			{
				checkTableSize();
				textTable[key] = val;
				return val;
			}
		}
		void cloneXMLNode(XMLNode& node, XMLCell& cell)
		{
			for (auto e = node.leaves().firstElem(); e.valid(); ++e)
			{
				XMLCell& newCell = cells.append(e->value());
				newCell.root = &cell;
				cell.leaves.addToTail(&newCell);
				cloneXMLNode(e.value(), newCell);
			}
		}
	};
}

namespace xml_internal
{
	using namespace spadas;
	using namespace spadas::console;

	const UInt STRING_STREAM_BUFFER_SIZE = 256;

	String quot = "\"";
	String space = " ";
	Binary emptyXmlStringBinary = String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n").toBinary();

	enum class BracketType
	{
		Dummy = 0,
		Start = 1,
		End = 2,
		Atom = 3,
	};

	class XmlCharsValidator
	{
	public:
		XmlCharsValidator()
		{
			for (UInt i = 0; i < 256; i++)
			{
				Char c = (Char)(Byte)i;
				flags[i] = (c >= '0' && c <= '9') ||
					(c >= 'A' && c <= 'Z') ||
					(c >= 'a' && c <= 'z') ||
					(c == '_') ||
					(c == '.') ||
					(c == '-') ||
					(c == ':');
			}
		}
		Bool validate(String& text)
		{
			UInt textLength = text.length();
			if (textLength == 0) return FALSE;
			const Byte *textData = text.bytes();
			for (UInt i = 0; i < textLength; i++)
			{
				if (!flags[textData[i]]) return FALSE;
			}
			return TRUE;
		}
		Bool validate(Byte c)
		{
			return flags[c];
		}
	private:
		Bool flags[256];
	}
	xmlCharsValidator;

	struct StringStreamElem
	{
		UInt size;
		Byte buffer[STRING_STREAM_BUFFER_SIZE];
		const Byte *extBuffer;
		StringStreamElem() : size(0)
		{}
		StringStreamElem(const Byte* extData, UInt size) : size(size), extBuffer(extData)
		{}
	};

	class StringStream
	{
	public:
		StringStream()
		{
			lastBuffer = &bufferList.addToTail(StringStreamElem());
		}
		void enqueue(const Byte* data, UInt size)
		{
			if (lastBuffer->size + size > STRING_STREAM_BUFFER_SIZE)
			{
				if (size > STRING_STREAM_BUFFER_SIZE)
				{
					lastBuffer = &bufferList.addToTail(StringStreamElem(data, size));
					return;
				}
				lastBuffer = &bufferList.addToTail(StringStreamElem());
			}
			utility::memoryCopy(data, &lastBuffer->buffer[lastBuffer->size], size);
			lastBuffer->size += size;
		}
		void enqueue(Binary& bin)
		{
			if (bin.size() > STRING_STREAM_BUFFER_SIZE)
			{
				largeBinaries.append(bin);
				lastBuffer = &bufferList.addToTail(StringStreamElem(bin.data(), bin.size()));
			}
			else
			{
				enqueue(bin.data(), bin.size());
			}
		}
		Binary dequeue()
		{
			UInt totalSize = 0;
			for (auto e = bufferList.head(); e.valid(); ++e)
			{
				totalSize += e->size;
			}

			Binary output(totalSize);
			Byte *outputData = output.data();
			UInt index = 0;
			for (auto e = bufferList.head(); e.valid(); ++e)
			{
				utility::memoryCopy(e->size > STRING_STREAM_BUFFER_SIZE ? e->extBuffer : e->buffer, &outputData[index], e->size);
				index += e->size;
			}

			bufferList.clear();
			lastBuffer = &bufferList.addToTail(StringStreamElem());
			largeBinaries = ArrayX<Binary>();

			return output;
		}
	private:
		List<StringStreamElem> bufferList;
		ArrayX<Binary> largeBinaries;
		StringStreamElem *lastBuffer;
	};

	void encodeES(String& text, Bool includeQuotation, Binary& esBuffer, StringStream& stream) // escape sequence
	{
		UInt textLength = text.length();
		const Byte *textData = text.bytes();
		UInt outLength = textLength;
		for (UInt i = 0; i < textLength; i++)
		{
			switch (textData[i])
			{
			case (Byte)'&':
				outLength += 4;
				break;
			case (Byte)'<':
			case (Byte)'>':
				outLength += 3;
				break;
			case (Byte)'\"':
				outLength += includeQuotation ? 5 : 0;
				break;
			}
		}

		if (outLength == 0) return;
		
		Binary buffer = outLength > STRING_STREAM_BUFFER_SIZE ? Binary(outLength) : esBuffer;
		Byte* outData = buffer.data();
		
		UInt k = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			switch (textData[i])
			{
			case (Byte)'&':
				outData[k++] = '&';
				outData[k++] = 'a';
				outData[k++] = 'm';
				outData[k++] = 'p';
				outData[k++] = ';';
				break;
			case (Byte)'<':
				outData[k++] = '&';
				outData[k++] = 'l';
				outData[k++] = 't';
				outData[k++] = ';';
				break;
			case (Byte)'>':
				outData[k++] = '&';
				outData[k++] = 'g';
				outData[k++] = 't';
				outData[k++] = ';';
				break;
			case (Byte)'\"':
				if (includeQuotation)
				{
					outData[k++] = '&';
					outData[k++] = 'q';
					outData[k++] = 'u';
					outData[k++] = 'o';
					outData[k++] = 't';
					outData[k++] = ';';
				}
				else outData[k++] = textData[i];
				break;
			default:
				outData[k++] = textData[i];
				break;
			}
		}
		SPADAS_ERROR_RETURN(k != outLength);

		if (outLength > STRING_STREAM_BUFFER_SIZE)
		{
			buffer.trim(outLength);
			stream.enqueue(buffer);
		}
		else
		{
			stream.enqueue(outData, outLength);
		}
	}

	Bool writeTagWithoutRightBracket(XMLElement& element, Binary& esBuffer, StringStream& stream)
	{
		SPADAS_ERROR_RETURNVAL(!xmlCharsValidator.validate(element.tag), FALSE);

		stream.enqueue((const Byte*)"<", 1);
		stream.enqueue(element.tag.bytes(), element.tag.length());

		if (!element.attributes.isEmpty())
		{
			stream.enqueue((const Byte*)" ", 1);

			UInt lastIndex = element.attributes.size() - 1;
			for (auto e = element.attributes.firstElem(); e.valid(); ++e)
			{
				SPADAS_ERROR_RETURNVAL(!xmlCharsValidator.validate(e->name), FALSE);
				stream.enqueue(e->name.bytes(), e->name.length());
				stream.enqueue((const Byte*)"=\"", 2);
				encodeES(e->value, TRUE, esBuffer, stream);
				stream.enqueue((const Byte*)"\"", 1);
				if (e.index() != lastIndex) stream.enqueue((const Byte*)" ", 1);
			}
		}

		return TRUE;
	}

	Bool writeXMLNodeToStream(XMLNode& node, UInt depth, Binary& esBuffer, StringStream& stream)
	{
		if (depth == 0)
		{
			stream.enqueue((const Byte*)"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n", 39);
		}
		else
		{
			for (UInt i = 0; i < depth; i++)
			{
				stream.enqueue((const Byte*)"\t", 1);
			}
		}

		XMLElement& element = node.value();
		SPADAS_ERROR_RETURNVAL(element.tag.isEmpty(), FALSE);
		if (!writeTagWithoutRightBracket(element, esBuffer, stream)) return FALSE;

		if (node.nLeaves() == 0)
		{
			if (element.content.isEmpty())
			{
				stream.enqueue((const Byte*)"/>\n", 3);
			}
			else
			{
				stream.enqueue((const Byte*)">", 1);
				encodeES(element.content, FALSE, esBuffer, stream);
				stream.enqueue((const Byte*)"</", 2);
				stream.enqueue(element.tag.bytes(), element.tag.length());
				stream.enqueue((const Byte*)">\n", 2);
			}
		}
		else
		{
			stream.enqueue((const Byte*)">\n", 2);
			for (auto e = node.leaves().firstElem(); e.valid(); ++e)
			{
				if (!writeXMLNodeToStream(e.value(), depth + 1, esBuffer, stream)) return FALSE;
			}
			stream.enqueue((const Byte*)"</", 2);
			stream.enqueue(element.tag.bytes(), element.tag.length());
			stream.enqueue((const Byte*)">\n", 2);
		}
		
		return TRUE;
	}
	
	String decodeES(StringSpan text, Binary& esBuffer) // escape sequence
	{
		UInt textLength = text.length();
		if (textLength == 0) return String();

		const Byte *textData = text.bytes();
		UInt decCount = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			if (textData[i] == '&')
			{
				if (i + 4 < textLength && textData[i+1] == 'a' && textData[i+2] == 'm' && textData[i+3] == 'p' && textData[i+4] == ';') { decCount += 4; i += 4; }
				else if (i + 3 < textLength && textData[i+1] == 'l' && textData[i+2] == 't' && textData[i+3] == ';') { decCount += 3; i += 3; }
				else if (i + 3 < textLength && textData[i+1] == 'g' && textData[i+2] == 't' && textData[i+3] == ';') { decCount += 3; i += 3; }
				else if (i + 5 < textLength && textData[i+1] == 'q' && textData[i+2] == 'u' && textData[i+3] == 'o' && textData[i+4] == 't' && textData[i+5] == ';') { decCount += 5; i += 5; }
			}
		}
		if (decCount == 0) return text.clone();
		
		UInt bufferSize = textLength - decCount;
		Binary buffer = bufferSize > STRING_STREAM_BUFFER_SIZE ? Binary(bufferSize) : esBuffer;
		Byte* outData = buffer.data();
		
		UInt k = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			if (textData[i] == '&')
			{
				if (i + 4 < textLength && textData[i+1] == 'a' && textData[i+2] == 'm' && textData[i+3] == 'p' && textData[i+4] == ';') { outData[k++] = '&'; i += 4; }
				else if (i + 3 < textLength && textData[i+1] == 'l' && textData[i+2] == 't' && textData[i+3] == ';') { outData[k++] = '<'; i += 3; }
				else if (i + 3 < textLength && textData[i+1] == 'g' && textData[i+2] == 't' && textData[i+3] == ';') { outData[k++] = '>'; i += 3; }
				else if (i + 5 < textLength && textData[i+1] == 'q' && textData[i+2] == 'u' && textData[i+3] == 'o' && textData[i+4] == 't' && textData[i+5] == ';') { outData[k++] = '\"'; i += 5; }
				else outData[k++] = '&';
			}
			else outData[k++] = textData[i];
		}
		SPADAS_ERROR_RETURNVAL(k != bufferSize, String());

		return BinarySpan(buffer, 0, bufferSize);
	}
	
	Bool extractTag(StringSpan bracketContent, XMLVars *xmlVars, String& tag, StringSpan& attributesString)
	{
		Array<UInt> spaceLocations = bracketContent.search(space);
		if (spaceLocations.size() == 0)
		{
			SPADAS_ERROR_RETURNVAL(bracketContent.isEmpty(), FALSE);
			tag = xmlVars->touchText(bracketContent);
		}
		else
		{
			SPADAS_ERROR_RETURNVAL(spaceLocations[0] == 0, FALSE);
			StringSpan span = bracketContent.sub(0, spaceLocations[0]);
			tag = xmlVars->touchText(span);
			attributesString = bracketContent.sub(spaceLocations[0] + 1);
		}
		return TRUE;
	}

	Array<XMLAttribute> unpackAttributes(StringSpan& attributesString, Binary& esBuffer, XMLVars *xmlVars)
	{
		Array<StringSpan> subStrings = attributesString.split(quot);
		if (subStrings.size() < 2) return Array<XMLAttribute>();

		UInt outSize = subStrings.size() / 2;
		Array<XMLAttribute> out(outSize);
		XMLAttribute *outData = out.data();
		StringSpan *subStringsData = subStrings.data();
		UInt nAttribs = 0;
		for (UInt i = 0; i < outSize; i++)
		{
			StringSpan& nameSpan = subStringsData[2 * i];
			StringSpan& valueSpan = subStringsData[2 * i + 1];

			const Byte *nameStringData = nameSpan.bytes();
			UInt nameStringLength = nameSpan.length();

			UInt spaceCount = 0;
			for (; spaceCount < nameStringLength; spaceCount++)
			{
				if (nameStringData[spaceCount] != (Byte)' ') break;
			}

			UInt validLength = 0;
			for (UInt charIndex = spaceCount; charIndex < nameStringLength; charIndex++, validLength++)
			{
				if (nameStringData[charIndex] == (Byte)'=') break;
				if (!xmlCharsValidator.validate(nameStringData[charIndex])) break;
			}
			if (validLength == 0) continue;

			XMLAttribute& attribute = outData[nAttribs++];
			StringSpan validNameSpan = nameSpan.sub(spaceCount, validLength);
			attribute.name = xmlVars->touchText(validNameSpan);
			attribute.value = decodeES(valueSpan, esBuffer);
		}
		if (nAttribs == 0) return Array<XMLAttribute>();

		out.trim(nAttribs);
		return out;
	}

	Bool readXMLBinary(String rawString, Binary& esBuffer, XML& xml)
	{
		/* split with the angle brackets */
		Array<UInt> leftRaw = rawString.search('<');
		Array<UInt> rightRaw = rawString.search('>');
		Array<UInt> leftAngleLocations(leftRaw.size());
		Array<UInt> rightAngleLocations(rightRaw.size());

		UInt *leftRawData = leftRaw.data(), *rightRawData = rightRaw.data();
		UInt *leftFineData = leftAngleLocations.data(), *rightFineData = rightAngleLocations.data();
		UInt leftRawSize = leftRaw.size(), rightRawSize = rightRaw.size();
		UInt nBrackets = 0, leftIndex = 0, rightIndex = 0;
		while (leftIndex < leftRawSize && rightIndex < rightRawSize)
		{
			while (leftIndex + 1 < leftRawSize && leftRawData[leftIndex + 1] < rightRawData[rightIndex]) leftIndex++;
			while (rightRawData[rightIndex] < leftRawData[leftIndex])
			{
				if (rightIndex + 1 >= rightRawSize) break;
				rightIndex++;
			}
			leftFineData[nBrackets] = leftRawData[leftIndex++];
			rightFineData[nBrackets++] = rightRawData[rightIndex++];
		}

		Array<BracketType> bracketTypes(nBrackets);
		Array<StringSpan> bracketContents(nBrackets);
		Array<String> externalContents(nBrackets);
		
		UInt startBracketNum = 0;
		UInt endBracketNum = 0;
		
		const Byte* rawStringData = rawString.bytes();
		for (UInt i = 0; i < nBrackets; i++)
		{
			SPADAS_ERROR_RETURNVAL((i != 0 && leftAngleLocations[i] < rightAngleLocations[i - 1]) || leftAngleLocations[i] > rightAngleLocations[i], FALSE);

			/* get bracket type */
			Byte leftAngleSymbol = rawStringData[leftAngleLocations[i]+1];
			Byte rightAngleSymbol = rawStringData[rightAngleLocations[i]-1];
			
			if (leftAngleSymbol == '?' || leftAngleSymbol == '!' ||
				rightAngleSymbol == '?' || rightAngleSymbol == '!' ||
				(leftAngleSymbol == '/' && rightAngleSymbol == '/'))
			{
				bracketTypes[i] = BracketType::Dummy;
			}
			else if (leftAngleSymbol == '/' && rightAngleSymbol != '/')
			{
				endBracketNum++;
				bracketTypes[i] = BracketType::End;
			}
			else if (leftAngleSymbol != '/' && rightAngleSymbol == '/')
			{
				bracketTypes[i] = BracketType::Atom;
			}
			else
			{
				startBracketNum++;
				bracketTypes[i] = BracketType::Start;
			}
			
			/* get bracket content */
			switch (bracketTypes[i])
			{
				case BracketType::Start:
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-1);
					break;
					
				case BracketType::End:
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+2, rightAngleLocations[i]-leftAngleLocations[i]-2);
					break;
					
				case BracketType::Atom:
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-2);
					break;
					
				default:
					break;
			}
			
			/* get external content */
			if (i != nBrackets - 1)
			{
				externalContents[i] = decodeES(rawString.sub(rightAngleLocations[i]+1, leftAngleLocations[i+1]-rightAngleLocations[i]-1), esBuffer);
			}
		}
		
		SPADAS_ERROR_RETURNVAL(startBracketNum != endBracketNum, FALSE);

		/* generate an XML tree */
		XMLVars *xmlVars = xml.getVars();
		XMLNode root = xml.globalRoot();
		XMLNode current;
		UInt rootCount = 0;
		String tag;
		StringSpan attributesString;

		for (UInt i = 0; i < nBrackets; i++)
		{
			if (bracketTypes[i] == BracketType::Dummy) continue;
			
			if (bracketTypes[i] == BracketType::Start)
			{
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], xmlVars, tag, attributesString), FALSE);

				if (current.valid())
				{
					current = current.addLeaf(XMLElement(tag, unpackAttributes(attributesString, esBuffer, xmlVars), externalContents[i]));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					current = root;
					current->tag = tag;
					current->attributes = unpackAttributes(attributesString, esBuffer, xmlVars);
					current->content = externalContents[i];
				}
			}

			if (bracketTypes[i] == BracketType::End)
			{
				SPADAS_ERROR_RETURNVAL(bracketContents[i] != current->tag, FALSE);
				SPADAS_ERROR_RETURNVAL(!current.valid(), FALSE);
				current = current.root();
			}
			
			if (bracketTypes[i] == BracketType::Atom)
			{
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], xmlVars, tag, attributesString), FALSE);

				if (current.valid())
				{
					current.addLeaf(XMLElement(tag, unpackAttributes(attributesString, esBuffer, xmlVars), String()));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					root->tag = tag;
					root->attributes = unpackAttributes(attributesString, esBuffer, xmlVars);
					root->content = String();
				}
			}
		}

		return TRUE;
	}
}

using namespace xml_internal;

const String spadas::XML::TypeName = "spadas.XML";

// XMLAttribute ///////////////////////////////////////////////////////////////////////////

XMLAttribute::XMLAttribute()
{
}

XMLAttribute::XMLAttribute(String name0, String value0) : name(name0), value(value0)
{
}

// XMLElement ///////////////////////////////////////////////////////////////////////////

XMLElement::XMLElement() : tag(unknownTag)
{
}
XMLElement::XMLElement(String tag0) : tag(tag0.isEmpty() ? unknownTag : tag0)
{
}
XMLElement::XMLElement(String tag0, Array<XMLAttribute> attributes0, String content0) : tag(tag0.isEmpty() ? unknownTag : tag0), attributes(attributes0), content(content0)
{
}

// XMLNode ///////////////////////////////////////////////////////////////////////////

XMLNode::XMLNode() : cell(NULL)
{}

XMLNode::XMLNode(XML xml, Pointer cell) : xml(xml), cell(cell)
{}

Bool XMLNode::valid()
{
	return cell != NULL;
}

XMLElement& XMLNode::value()
{
	SPADAS_ERROR_RETURNVAL(!cell, *(new XMLElement()));
	return ((XMLCell*)cell)->elem;
}

XMLElement* XMLNode::operator ->()
{
	SPADAS_ERROR_RETURNVAL(!cell, new XMLElement());
	return &((XMLCell*)cell)->elem;
}

XMLNode XMLNode::root()
{
	SPADAS_ERROR_RETURNVAL(!cell, XMLNode());
	return XMLNode(xml, ((XMLCell*)cell)->root);
}

UInt XMLNode::nLeaves()
{
	SPADAS_ERROR_RETURNVAL(!cell, 0);
	return ((XMLCell*)cell)->leaves.size();
}

Array<XMLNode> XMLNode::leaves()
{
	SPADAS_ERROR_RETURNVAL(!cell, Array<XMLNode>());

	UInt leafCount = ((XMLCell*)cell)->leaves.size();
	if (leafCount == 0) return Array<XMLNode>();

	Array<XMLNode> output = Array<XMLNode>::createUninitialized(leafCount);
	for (auto e = ((XMLCell*)cell)->leaves.head(); e.valid(); ++e)
	{
		output.initialize(e.index(), XMLNode(xml, e.value()));
	}
	return output;
}

Array<XMLNode> XMLNode::leavesWithTagName(String tagName)
{
	SPADAS_ERROR_RETURNVAL(!cell, Array<XMLNode>());
	SPADAS_ERROR_RETURNVAL(tagName.isEmpty(), Array<XMLNode>());

	ArrayX<XMLNode> output;
	for (auto e = ((XMLCell*)cell)->leaves.head(); e.valid(); ++e)
	{
		if (e.value()->elem.tag == tagName) output.append(XMLNode(xml, e.value()));
	}
	return output.toArray();
}

Bool XMLNode::firstLeafWithTagName(String tagName, XMLNode& output)
{
	SPADAS_ERROR_RETURNVAL(!cell, FALSE);
	SPADAS_ERROR_RETURNVAL(tagName.isEmpty(), FALSE);

	for (auto e = ((XMLCell*)cell)->leaves.head(); e.valid(); ++e)
	{
		if (e.value()->elem.tag == tagName)
		{
			output = XMLNode(xml, e.value());
			return TRUE;
		}
	}
	return FALSE;
}

Dictionary<String> XMLNode::attributesToDictionary()
{
	SPADAS_ERROR_RETURNVAL(!cell, Dictionary<String>());

	Dictionary<String> out;
	for (auto e = ((XMLCell*)cell)->elem.attributes.firstElem(); e.valid(); ++e)
	{
		out[e->name] = e->value;
	}
	return out;
}

void XMLNode::dictionaryToAttributes(Dictionary<String> dict)
{
	SPADAS_ERROR_RETURN(!cell);

	ArrayX<XMLAttribute> buf;
	if (!dict.isEmpty())
	{
		for (auto pair = dict.keyValues().firstElem(); pair.valid(); ++pair)
		{
			if (pair->key.isEmpty()) continue;
			StringSpan keySpan(pair->key, 0, pair->key.length());
			String key = xml.getVars()->touchText(keySpan, pair->key);
			buf.append(XMLAttribute(key, pair->value));
		}
	}
	((XMLCell*)cell)->elem.attributes = buf.toArray();
}

XMLNode XMLNode::addLeaf(XMLElement val)
{
	SPADAS_ERROR_RETURNVAL(!cell, XMLNode());

	XMLCell *newCell = &xml.getVars()->cells.append(XMLCell(val));
	newCell->root = (XMLCell*)cell;
	((XMLCell*)cell)->leaves.addToTail(newCell);
	return XMLNode(xml, newCell);
}

// XML ///////////////////////////////////////////////////////////////////////////

XML::XML()
{
}

XML::XML(XMLNode xmlRoot) : Object<class XMLVars>(new XMLVars(xmlRoot), TRUE)
{
}

Optional<XML> XML::createFromFile(Path xmlFilePath)
{
	SPADAS_ERROR_RETURNVAL(xmlFilePath.isNull(), Optional<XML>());
	SPADAS_ERROR_RETURNVAL(!xmlFilePath.exist(), Optional<XML>());

	File file = File::openBinary(xmlFilePath);
    if (file.isNull()) return Optional<XML>();
    
	Binary binary = file.input();
    
	SPADAS_ERROR_RETURNVAL(binary.size() < 8, Optional<XML>());

	SPADAS_ERROR_RETURNVAL((binary[0] == 0xFE && binary[1] == 0xFF) || (binary[0] == 0xFF && binary[1] == 0xFE), Optional<XML>());
	SPADAS_ERROR_RETURNVAL((binary[0] == 0 && binary[1] == 0 && binary[2] == 0xFE && binary[3] == 0xFF) || (binary[0] == 0xFF && binary[1] == 0xFE && binary[2] == 0 && binary[3] == 0), Optional<XML>());

	BinarySpan span;
    if (binary[0] == 0xEF && binary[1] == 0xBB && binary[2] == 0xBF)
    {
		span = binary.sub(3);
    }
	else
	{
		SPADAS_WARNING_MSG("No BOM");
		span = binary.sub(0);
	}

	XML xml;
	xml.setVars(new XMLVars(), TRUE);
	Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
	if (readXMLBinary(span, esBuffer, xml)) return xml;
	else return Optional<XML>();
}

Optional<XML> XML::createFromBinary(Binary xmlBinary)
{
	XML xml;
	xml.setVars(new XMLVars(), TRUE);
	Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
	if (readXMLBinary(xmlBinary, esBuffer, xml)) return xml;
	else return Optional<XML>();
}

XML XML::clone()
{
	if (!vars) return XML();
    else return XML(globalRoot());
}

XMLNode XML::globalRoot()
{
	if (!vars) setVars(new XMLVars(), TRUE);
	return XMLNode(*this, &vars->cells[0]);
}

void XML::save(Path xmlFilePath)
{
	SPADAS_ERROR_RETURN(xmlFilePath.isNull());

	File file = File::createBinary(xmlFilePath);
	SPADAS_ERROR_RETURN(file.isNull());

	Binary bom(3);
	bom[0] = 0xEF;
	bom[1] = 0xBB;
	bom[2] = 0xBF;
	file.output(bom);

	if (!vars)
	{
		file.output(emptyXmlStringBinary);
	}
	else
	{
		StringStream stream;
		Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
		XMLNode rootNode = globalRoot();
		if (writeXMLNodeToStream(rootNode, 0, esBuffer, stream)) file.output(stream.dequeue());
		else file.output(emptyXmlStringBinary);
	}
}

Binary XML::toBinary()
{
	if (!vars)
	{
		return emptyXmlStringBinary.clone();
	}
	else
	{
		StringStream stream;
		Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
		XMLNode rootNode = globalRoot();
		if (writeXMLNodeToStream(rootNode, 0, esBuffer, stream)) return stream.dequeue();
		else return emptyXmlStringBinary.clone();
	}
}