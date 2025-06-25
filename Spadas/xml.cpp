
#include "spadas.h"

namespace spadas
{
	const UInt XML_CELL_ARRAY_SEGMENT_SIZE = 256;

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
		SPADAS_VARS(XML, Vars)

		ArrayX<XMLCell> cells; // 首个为根节点

		XMLVars() : cells(XML_CELL_ARRAY_SEGMENT_SIZE)
		{
			cells.append(XMLElement(rootTag, 0, String()));
		}
		XMLVars(XMLNode rootNode) : cells(XML_CELL_ARRAY_SEGMENT_SIZE)
		{
			XMLCell& rootCell = cells.append(rootNode.value());
			cloneXMLNode(rootNode, rootCell);
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

	Binary emptyXmlStringBinary = String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n").toBinary();

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
		void enqueueChar(Char c)
		{
			if (lastBuffer->size >= STRING_STREAM_BUFFER_SIZE)
			{
				lastBuffer = &bufferList.addToTail(StringStreamElem());
			}
			lastBuffer->buffer[lastBuffer->size++] = (Byte)c;
		}
		void enqueueSmall(const Byte* data, UInt size)
		{
			if (lastBuffer->size + size > STRING_STREAM_BUFFER_SIZE)
			{
				lastBuffer = &bufferList.addToTail(StringStreamElem());
			}
			Byte* target = &lastBuffer->buffer[lastBuffer->size];
			for (UInt i = 0; i < size; i++)
			{
				*target++ = *data++;
			}
			lastBuffer->size += size;
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
			for (auto e = bufferList.headElem(); e.valid(); ++e)
			{
				totalSize += e->size;
			}

			Binary output(totalSize);
			Byte *outputData = output.data();
			UInt index = 0;
			for (auto e = bufferList.headElem(); e.valid(); ++e)
			{
				utility::memoryCopy(e->size > STRING_STREAM_BUFFER_SIZE ? e->extBuffer : e->buffer, &outputData[index], e->size);
				index += e->size;
			}

			bufferList.clear();
			lastBuffer = &bufferList.addToTail(StringStreamElem());
			largeBinaries.clear();

			return output;
		}
	private:
		List<StringStreamElem> bufferList;
		ArrayX<Binary> largeBinaries;
		StringStreamElem *lastBuffer;
	};

	void encodeES(String& text, Bool attributeName, Binary& esBuffer, StringStream& stream) // escape sequence
	{
		UInt textLength = text.length();
		if (textLength == 0) return;

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
				outLength += attributeName ? 5 : 0;
				break;
			case (Byte)'\r':
			case (Byte)'\n':
				outLength += attributeName ? 4 : 0;
				break;
			}
		}

		if (outLength == textLength)
		{
			stream.enqueue(textData, textLength);
			return;
		}
		
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
				if (attributeName)
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
			case (Byte)'\r':
				if (attributeName)
				{
					outData[k++] = '&';
					outData[k++] = '#';
					outData[k++] = 'x';
					outData[k++] = 'D';
					outData[k++] = ';';
				}
				else outData[k++] = textData[i];
				break;
			case (Byte)'\n':
				if (attributeName)
				{
					outData[k++] = '&';
					outData[k++] = '#';
					outData[k++] = 'x';
					outData[k++] = 'A';
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

		stream.enqueueChar('<');
		stream.enqueue(element.tag.bytes(), element.tag.length());

		if (!element.attributes.isEmpty())
		{
			stream.enqueueChar(' ');

			UInt lastIndex = element.attributes.size() - 1;
			for (auto e = element.attributes.firstElem(); e.valid(); ++e)
			{
				SPADAS_ERROR_RETURNVAL(!xmlCharsValidator.validate(e->name), FALSE);
				stream.enqueue(e->name.bytes(), e->name.length());
				stream.enqueueSmall((const Byte*)"=\"", 2);
				encodeES(e->value, TRUE, esBuffer, stream);
				stream.enqueueChar('\"');
				if (e.index() != lastIndex) stream.enqueueChar(' ');
			}
		}

		return TRUE;
	}

	Bool writeXMLNodeToStream(XMLNode& node, UInt depth, Binary& esBuffer, StringStream& stream)
	{
		if (depth == 0)
		{
			stream.enqueueSmall((const Byte*)"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n", 39);
		}
		else
		{
			for (UInt i = 0; i < depth; i++)
			{
				stream.enqueueChar('\t');
			}
		}

		XMLElement& element = node.value();
		SPADAS_ERROR_RETURNVAL(element.tag.isEmpty(), FALSE);
		if (!writeTagWithoutRightBracket(element, esBuffer, stream)) return FALSE;

		if (node.nLeaves() == 0)
		{
			if (element.content.isEmpty())
			{
				stream.enqueueSmall((const Byte*)"/>\n", 3);
			}
			else
			{
				stream.enqueueChar('>');
				encodeES(element.content, FALSE, esBuffer, stream);
				stream.enqueueSmall((const Byte*)"</", 2);
				stream.enqueue(element.tag.bytes(), element.tag.length());
				stream.enqueueSmall((const Byte*)">\n", 2);
			}
		}
		else
		{
			stream.enqueueSmall((const Byte*)">\n", 2);
			for (auto e = node.leaves().firstElem(); e.valid(); ++e)
			{
				if (!writeXMLNodeToStream(e.value(), depth + 1, esBuffer, stream)) return FALSE;
			}
			for (UInt i = 0; i < depth; i++)
			{
				stream.enqueueChar('\t');
			}
			stream.enqueueSmall((const Byte*)"</", 2);
			stream.enqueue(element.tag.bytes(), element.tag.length());
			stream.enqueueSmall((const Byte*)">\n", 2);
		}
		
		return TRUE;
	}
	
	String decodeES(StringSpan& text, Binary& esBuffer) // escape sequence
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
				else if (i + 4 < textLength && textData[i+1] == '#' && textData[i+2] == 'x' && textData[i+3] == 'D' && textData[i+4] == ';') { decCount += 4; i += 4; }
				else if (i + 4 < textLength && textData[i+1] == '#' && textData[i+2] == 'x' && textData[i+3] == 'A' && textData[i+4] == ';') { decCount += 4; i += 4; }
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
				else if (i + 4 < textLength && textData[i+1] == '#' && textData[i+2] == 'x' && textData[i+3] == 'D' && textData[i+4] == ';') { outData[k++] = '\r'; i += 4; }
				else if (i + 4 < textLength && textData[i+1] == '#' && textData[i+2] == 'x' && textData[i+3] == 'A' && textData[i+4] == ';') { outData[k++] = '\n'; i += 4; }
				else outData[k++] = '&';
			}
			else outData[k++] = textData[i];
		}
		SPADAS_ERROR_RETURNVAL(k != bufferSize, String());

		return BinarySpan(buffer, 0, bufferSize);
	}

	Array<XMLAttribute> unpackAttributes(StringSpan& attributesString, UInt quotCount, Binary& esBuffer)
	{
		UInt outSize = quotCount / 2;
		Array<XMLAttribute> out(outSize);
		XMLAttribute *outData = out.data();
		const Byte *chars = attributesString.bytes();
		UInt totalLength = attributesString.length();
		UInt nAttribs = 0;
		UInt curIndex = 0;

		while (TRUE)
		{
			// Find valid character
			while (curIndex < totalLength && !xmlCharsValidator.validate(chars[curIndex]))
			{
				curIndex++;
				continue;
			}
			if (curIndex >= totalLength) break;

			// Key
			UInt keyIndex = curIndex++;
			UInt keyLength = 1;
			while (curIndex < totalLength)
			{
				if (chars[curIndex] == (Byte)'=') break;
				if (xmlCharsValidator.validate(chars[curIndex])) keyLength++;
				curIndex++;
			}
			String key = attributesString.span(keyIndex, keyLength).clone();

			// Quot
			UInt leftQuotIndex = ++curIndex;
			SPADAS_ERROR_RETURNVAL(leftQuotIndex >= totalLength || chars[leftQuotIndex] != '\"', Array<XMLAttribute>());

			// Value
			UInt valueIndex = ++curIndex;
			UInt valueLength = 0;
			while (TRUE)
			{
				SPADAS_ERROR_RETURNVAL(curIndex >= totalLength, FALSE);
				if (chars[curIndex] == (Byte)'\"') break;
				valueLength++;
				curIndex++;
			}

			String value;
			if (valueLength > 0)
			{
				StringSpan valueSpan = attributesString.span(valueIndex, valueLength);
				value = decodeES(valueSpan, esBuffer);
			}

			// Append
			outData[nAttribs].name = key;
			outData[nAttribs++].value = value;
			curIndex++;
		}
		if (nAttribs == 0) return Array<XMLAttribute>();

		out.trim(nAttribs);
		return out;
	}

	Bool readXMLBinary(StringSpan rawString, Binary& esBuffer, XML& xml)
	{
		XMLNode root = xml.globalRoot();
		XMLNode current;
		UInt rootCount = 0;
		const Byte *chars = rawString.bytes();
		UInt totalLength = rawString.length();
		UInt curIndex = 0;
		UInt contentIndex = UINF;

		while (TRUE)
		{
			// Find '<'
			while (curIndex < totalLength && chars[curIndex] != (Byte)'<')
			{
				curIndex++;
				continue;
			}

			curIndex++;
			if (curIndex >= totalLength) break;

			if (chars[curIndex] == (Byte)'?' || chars[curIndex] == (Byte)'!')
			{
				curIndex++;
				continue;
			}

			// Ender
			if (chars[curIndex] == (Byte)'/')
			{
				SPADAS_ERROR_RETURNVAL(!current.valid(), FALSE);

				UInt enderTagIndex = ++curIndex;
				UInt enderTagLength = 0;
				while (TRUE)
				{
					SPADAS_ERROR_RETURNVAL(curIndex >= totalLength, FALSE);
					if (chars[curIndex] == (Byte)'>') break;
					if (xmlCharsValidator.validate(chars[curIndex])) enderTagLength++;
					curIndex++;
				}
				SPADAS_ERROR_RETURNVAL(enderTagLength == 0, FALSE);

				StringSpan enderTagSpan(rawString, enderTagIndex, enderTagLength);
				SPADAS_ERROR_RETURNVAL(enderTagSpan != current->tag, FALSE);

				if (current.nLeaves() == 0)
				{
					Int contentLength = (Int)enderTagIndex - (Int)contentIndex - 2;
					if (contentLength > 0)
					{
						StringSpan contentSpan(rawString, contentIndex, contentLength);
						current->content = decodeES(contentSpan, esBuffer);
					}
				}

				current = current.root();
				continue;
			}

			// Starter or atom
			UInt tagIndex = curIndex;
			UInt tagLength = 0;
			while (TRUE)
			{
				SPADAS_ERROR_RETURNVAL(curIndex >= totalLength, FALSE);
				if (chars[curIndex] == (Byte)' ' || chars[curIndex] == (Byte)'>') break;
				if (xmlCharsValidator.validate(chars[curIndex])) tagLength++;
				curIndex++;
			}
			SPADAS_ERROR_RETURNVAL(tagLength == 0, FALSE);
			String tag = StringSpan(rawString, tagIndex, tagLength).clone();

			if (chars[curIndex] == (Byte)'>')
			{
				// Atom (no attributes)
				if (chars[curIndex - 1] == (Byte)'/')
				{
					if (current.valid())
					{
						current.addLeaf(XMLElement(tag, 0, String()));
					}
					else
					{
						rootCount++;
						SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
						root->tag = tag;
					}

					curIndex++;
					continue;
				}

				// Starter (no attributes)
				if (current.valid())
				{
					current = current.addLeaf(XMLElement(tag, 0, String()/* fill later */));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					current = root;
					current->tag = tag;
				}

				contentIndex = ++curIndex;
				continue;
			}

			// Attributes
			UInt attributesIndex = ++curIndex;
			UInt attributesLength = 0;
			UInt quotCount = 0;
			while (TRUE)
			{
				SPADAS_ERROR_RETURNVAL(curIndex >= totalLength, FALSE);
				if (chars[curIndex] == (Byte)'>') break;
				if (chars[curIndex] == (Byte)'\"') quotCount++;
				attributesLength++;
				curIndex++;
			}

			Array<XMLAttribute> attributes;
			if (quotCount >= 2)
			{
				StringSpan attributesSpan(rawString, attributesIndex, attributesLength);
				attributes = unpackAttributes(attributesSpan, quotCount, esBuffer);
			}

			// Atom (maybe with attributes)
			if (chars[curIndex - 1] == (Byte)'/')
			{
				if (current.valid())
				{
					current.addLeaf(XMLElement(tag, attributes, String()));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					root->tag = tag;
					root->attributes = attributes;
				}

				curIndex++;
				continue;
			}

			// Starter (maybe with attributes)
			if (current.valid())
			{
				current = current.addLeaf(XMLElement(tag, attributes, String()/* fill later */));
			}
			else
			{
				rootCount++;
				SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
				current = root;
				current->tag = tag;
				current->attributes = attributes;
			}

			contentIndex = ++curIndex;
		}

		return TRUE;
	}
}

using namespace xml_internal;

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
	for (auto e = ((XMLCell*)cell)->leaves.headElem(); e.valid(); ++e)
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
	for (auto e = ((XMLCell*)cell)->leaves.headElem(); e.valid(); ++e)
	{
		if (e.value()->elem.tag == tagName) output.append(XMLNode(xml, e.value()));
	}
	return output.toArray();
}

Bool XMLNode::firstLeafWithTagName(String tagName, XMLNode& output)
{
	SPADAS_ERROR_RETURNVAL(!cell, FALSE);
	SPADAS_ERROR_RETURNVAL(tagName.isEmpty(), FALSE);

	for (auto e = ((XMLCell*)cell)->leaves.headElem(); e.valid(); ++e)
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
			buf.append(XMLAttribute(pair->key, pair->value));
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

XMLNode XMLNode::addLeaf(String tagName)
{
	return addLeaf(XMLElement(tagName));
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
    
	SPADAS_ERROR_RETURNVAL(binary.size() <= 4, Optional<XML>());

	SPADAS_ERROR_RETURNVAL((binary[0] == 0xFE && binary[1] == 0xFF) || (binary[0] == 0xFF && binary[1] == 0xFE), Optional<XML>());
	SPADAS_ERROR_RETURNVAL((binary[0] == 0 && binary[1] == 0 && binary[2] == 0xFE && binary[3] == 0xFF) || (binary[0] == 0xFF && binary[1] == 0xFE && binary[2] == 0 && binary[3] == 0), Optional<XML>());

	StringSpan span;
    if (binary[0] == 0xEF && binary[1] == 0xBB && binary[2] == 0xBF)
    {
		span = StringSpan(&binary[3], binary.size() - 3);
    }
	else
	{
		SPADAS_WARNING_MSG("No BOM");
		span = StringSpan(binary.data(), binary.size());
	}

	XML xml;
	xml.setVars(new XMLVars(), TRUE);
	Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
	if (readXMLBinary(span, esBuffer, xml)) return xml;
	else return Optional<XML>();
}

Optional<XML> XML::createFromBinary(Binary xmlBinary)
{
	SPADAS_ERROR_RETURNVAL(xmlBinary.size() <= 4, Optional<XML>());

	XML xml;
	xml.setVars(new XMLVars(), TRUE);
	Binary esBuffer(STRING_STREAM_BUFFER_SIZE);
	if (readXMLBinary(StringSpan(xmlBinary.data(), xmlBinary.size()), esBuffer, xml)) return xml;
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

Array<XMLNode> XML::nodeLeavesWithTagName(XMLNode node, String tagName)
{
	return node.leavesWithTagName(tagName);
}

Bool XML::firstNodeLeafWithTagName(XMLNode node, String tagName, XMLNode& output)
{
	return node.firstLeafWithTagName(tagName, output);
}

Dictionary<String> XML::attributesToDictionary(Array<XMLAttribute> attributes)
{
	UInt nAttribs = attributes.size();
	Dictionary<String> out;

	XMLAttribute *attribsData = attributes.data();
	for (UInt i = 0; i < nAttribs; i++)
	{
		out[attribsData[i].name] = attribsData[i].value;
	}
	return out;
}

Array<XMLAttribute> XML::dictionaryToAttributes(Dictionary<String> dict)
{
	Array<String> keys = dict.keys();
	ArrayX<XMLAttribute> buf;

	UInt nKeys = keys.size();
	for (UInt i = 0; i < nKeys; i++)
	{
		if (!keys[i].isEmpty()) buf[buf.size()] = XMLAttribute(keys[i], dict[keys[i]]);
	}
	return buf.toArray(Region(0, buf.size()));
}