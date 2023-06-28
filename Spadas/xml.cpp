
#include "spadas.h"

namespace xml_internal
{
	using namespace spadas;
	using namespace spadas::console;

	enum class BracketType
	{
		Dummy = 0,
		Start = 1,
		End = 2,
		Atom = 3,
	};
	
	String debug_stringBracketType(BracketType type)
	{
		switch (type)
		{
		case BracketType::Dummy:
				return "Dummy";
			case BracketType::Start:
				return "Start";
			case BracketType::End:
				return "End";
			case BracketType::Atom:
				return "Atom";
		}
		return "Dummy";
	}

	Bool okForTagOrAttributeName(String text)
	{
		UInt textLength = text.length();
		const Byte *textData = text.bytes();
		for (UInt i = 0; i < textLength; i++)
		{
			if ((textData[i] >= '0' && textData[i] <= '9') ||
				(textData[i] >= 'A' && textData[i] <= 'Z') ||
				(textData[i] >= 'a' && textData[i] <= 'z') ||
				(textData[i] == '_') ||
				(textData[i] == '.') ||
				(textData[i] == '-') ||
				(textData[i] == ':')) continue;
			return FALSE;
		}
		return TRUE;
	}
	
	// escape sequence
	String encodeES(String text, Bool includeQuotation)
	{
		UInt textLength = text.length();
		const Byte *textData = text.bytes();
		UInt countAmp = 0, countLt = 0, countGt = 0, countQuot = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			if (textData[i] == '&') countAmp++;
			else if (textData[i] == '<') countLt++;
			else if (textData[i] == '>') countGt++;
			else if (includeQuotation && textData[i] == '\"') countQuot++;
		}
		
		Binary buffer(textLength + countAmp * 4 + countQuot * 5 + countLt * 3 + countGt * 3);
		if (buffer.isEmpty()) return String();

		Byte* outData = buffer.data();
		
		UInt k = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			Byte character = textData[i];
			if (character == '&')
			{
				outData[k++] = '&';
				outData[k++] = 'a';
				outData[k++] = 'm';
				outData[k++] = 'p';
				outData[k++] = ';';
			}
			else if (character == '<')
			{
				outData[k++] = '&';
				outData[k++] = 'l';
				outData[k++] = 't';
				outData[k++] = ';';
			}
			else if (character == '>')
			{
				outData[k++] = '&';
				outData[k++] = 'g';
				outData[k++] = 't';
				outData[k++] = ';';
			}
			else if (includeQuotation && character == '\"')
			{
				outData[k++] = '&';
				outData[k++] = 'q';
				outData[k++] = 'u';
				outData[k++] = 'o';
				outData[k++] = 't';
				outData[k++] = ';';
			}
			else outData[k++] = character;
		}
		if (k == 0) return String();

		buffer.trim(k);
		return buffer;
	}
	
	String decodeES(String text)
	{
		UInt textLength = text.length();
		const Byte *textData = text.bytes();
		UInt countAmp = 0, countLt = 0, countGt = 0, countQuot = 0;
		for (UInt i = 0; i < textLength; i++)
		{
			if (textData[i] == '&')
			{
				if (i + 4 < textLength && textData[i+1] == 'a' && textData[i+2] == 'm' && textData[i+3] == 'p' && textData[i+4] == ';') { countAmp++; i += 4; }
				else if (i + 3 < textLength && textData[i+1] == 'l' && textData[i+2] == 't' && textData[i+3] == ';') { countLt++; i += 3; }
				else if (i + 3 < textLength && textData[i+1] == 'g' && textData[i+2] == 't' && textData[i+3] == ';') { countGt++; i += 3; }
				else if (i + 5 < textLength && textData[i+1] == 'q' && textData[i+2] == 'u' && textData[i+3] == 'o' && textData[i+4] == 't' && textData[i+5] == ';') { countQuot++; i += 5; }
			}
		}
		
		Binary buffer(textLength - countAmp * 4 - countQuot * 5 - countLt * 3 - countGt * 3);
		if (buffer.isEmpty()) return String();

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
		if (k == 0) return String();

		buffer.trim(k);
		return buffer;
	}
	
	Bool extractTag(String bracketContent, String& tag, String& attributesString)
	{
		Array<UInt> spaceLocations = bracketContent.search(' ');
		if (spaceLocations.size() == 0) tag = bracketContent.clone();
		else
		{
			tag = bracketContent.sub(0, spaceLocations[0]).clone();
			attributesString = bracketContent.sub(spaceLocations[0]+1).clone();
		}
		
		if (tag.isEmpty()) return FALSE;
		else return TRUE;
	}

	Array<XMLAttribute> unpackAttributes(String attributesString)
	{
		Array<StringSpan> subStrings = attributesString.split("\"");
		if (subStrings.size() < 2) return Array<XMLAttribute>();

		UInt outSize = subStrings.size() / 2;
		Array<XMLAttribute> out(outSize);
		UInt nAttribs = 0;
		for (UInt i = 0; i < outSize; i++)
		{
			String nameString = subStrings[2 * i].clone();
			String valueString = subStrings[2 * i + 1].clone();

			if (nameString.isEmpty()) continue;

			Binary buffer(nameString.length());
			Byte *attributeNameData = buffer.data();

			UInt k = 0;
			const Byte *nameStringData = nameString.bytes();
			UInt nameStringLength = nameString.length();
			for (UInt n = 0; n < nameStringLength; n++)
			{
				if (nameStringData[n] == '=') break;
				
				if ((nameStringData[n] >= '0' && nameStringData[n] <= '9') ||
					(nameStringData[n] >= 'A' && nameStringData[n] <= 'Z') ||
					(nameStringData[n] >= 'a' && nameStringData[n] <= 'z') ||
					nameStringData[n] == '_' || nameStringData[n] == '.')
				{
					attributeNameData[k++] = nameStringData[n];
				}
			}
			if (k == 0) continue;

			buffer.trim(k);

			XMLAttribute& attribute = out[nAttribs++];
			attribute.name = buffer;
			attribute.value = decodeES(valueString);
		}
		if (nAttribs == 0) return Array<XMLAttribute>();

		out.trim(nAttribs);
		return out;
	}

	String packAttributes(Array<XMLAttribute> attributes)
	{
		String out;
		UInt size = attributes.size();
		for (UInt i = 0; i < size; i++)
		{
			out += attributes[i].name;
			out += "=\"";
			out += encodeES(attributes[i].value, TRUE);
			out += "\"";
			if (i != size - 1) out += " ";
		}
		return out;
	}
	
	Bool readXMLBinary(BinarySpan xmlBinary, XML& xml)
	{
		String rawString(xmlBinary.clone());
		
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
		Array<String> bracketContents(nBrackets);
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
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-1).clone();
					break;
					
				case BracketType::End:
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+2, rightAngleLocations[i]-leftAngleLocations[i]-2).clone();
					break;
					
				case BracketType::Atom:
					bracketContents[i] = rawString.sub(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-2).clone();
					break;
					
				default:
					break;
			}
			
			/* get external content */
			if (i != nBrackets - 1)
			{
				externalContents[i] = decodeES(rawString.sub(rightAngleLocations[i]+1, leftAngleLocations[i+1]-rightAngleLocations[i]-1).clone());
			}
		}
		
		SPADAS_ERROR_RETURNVAL(startBracketNum != endBracketNum, FALSE);
		
		/* generate an XML tree */
		XMLNode root = xml.globalRoot();
		XMLNode current;
		UInt rootCount = 0;

		for (UInt i = 0; i < nBrackets; i++)
		{
			if (bracketTypes[i] == BracketType::Dummy) continue;
			
			if (bracketTypes[i] == BracketType::Start)
			{
				String tag, attributesString;
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], tag, attributesString), FALSE);

				if (current.valid())
				{
					current = current.addLeaf(XMLElement(tag, unpackAttributes(attributesString), externalContents[i]));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					current = root;
					current->tag = tag;
					current->attributes = unpackAttributes(attributesString);
					current->content = externalContents[i];
				}
			}

			if (bracketTypes[i] == BracketType::End)
			{
				SPADAS_ERROR_RETURNVAL(bracketContents[i] != current.value().tag, FALSE);
				SPADAS_ERROR_RETURNVAL(!current.valid(), FALSE);
				current = current.root();
			}
			
			if (bracketTypes[i] == BracketType::Atom)
			{
				String tag, attributesString;
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], tag, attributesString), FALSE);

				if (current.valid())
				{
					current.addLeaf(XMLElement(tag, unpackAttributes(attributesString), String()));
				}
				else
				{
					rootCount++;
					SPADAS_ERROR_RETURNVAL(rootCount > 1, FALSE);
					root->tag = tag;
					root->attributes = unpackAttributes(attributesString);
					root->content = String();
				}
			}
		}

		return TRUE;
	}
	
	Binary writeXMLNodeBinary(XMLNode node, UInt depth)
	{
		XMLElement element = node.value();
		
		// validate
		SPADAS_ERROR_RETURNVAL(!okForTagOrAttributeName(element.tag), Binary());

		for (UInt i = 0; i < element.attributes.size(); i++)
		{
			SPADAS_ERROR_RETURNVAL(!okForTagOrAttributeName(element.attributes[i].name), Binary());
		}
		
		String startBracketContent;
		if (element.attributes.isEmpty()) startBracketContent = element.tag.clone();
		else startBracketContent = element.tag + " " + packAttributes(element.attributes);
		
		String tabText;
		for (UInt i = 0; i < depth; i++) tabText += "	";

		String lineEnding = "\n";
		
		Binary out;
		if (node.nLeaves() == 0)
		{
			String outString;
			if (element.content.isEmpty())
			{
				outString = tabText + "<" + startBracketContent + "/>" + lineEnding;
			}
			else
			{
				outString = tabText + "<" + startBracketContent + ">" +
				encodeES(element.content, FALSE) +
				"</" + element.tag + ">" + lineEnding;
			}
			
			if (depth == 0) out = String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n").toBinary() + outString.toBinary();
			else out = outString.toBinary();
		}
		else
		{
			UInt size = 0, index = 0;

			Binary headerBinary;
			if (depth == 0) headerBinary = String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n").toBinary();
			size += headerBinary.size();
			
			Binary startBinary = String(tabText + "<" + startBracketContent + ">" + lineEnding).toBinary();
			size += startBinary.size();
			
			Array<XMLNode> leaves = node.leaves();
			Array<Binary> leavesBinaries(node.nLeaves());
			for (UInt i = 0; i < node.nLeaves(); i++)
			{
				leavesBinaries[i] = writeXMLNodeBinary(leaves[i], depth + 1);
				size += leavesBinaries[i].size();
			}
			
			Binary endBinary = String(tabText + "</" + element.tag + ">" + lineEnding).toBinary();
			size += endBinary.size();
			
			out = Binary(size);

			if (!headerBinary.isEmpty())
			{
				utility::memoryCopy(headerBinary.data(), &out[index], headerBinary.size());
				index += headerBinary.size();
			}
			
			utility::memoryCopy(startBinary.data(), &out[index], startBinary.size());
			index += startBinary.size();
			
			for (UInt i = 0; i < node.nLeaves(); i++)
			{
				utility::memoryCopy(leavesBinaries[i].data(), &out[index], leavesBinaries[i].size());
				index += leavesBinaries[i].size();
			}
			
			utility::memoryCopy(endBinary.data(), &out[index], endBinary.size());
		}
		
		return out;
	}
}

namespace spadas
{
	const UInt XML_CELL_ARRAY_SEGMENT_SIZE = 256;
	const UInt XML_TEXT_TABLE_SIZE_SMALL = 64;
	const UInt XML_TEXT_TABLE_COUNT_THRESH = 256;
	const UInt XML_TEXT_TABLE_SIZE_LARGE = 4096;

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
			cells.append(XMLElement("root", 0, String()));
		}
		XMLVars(XMLNode rootNode) : cells(XML_CELL_ARRAY_SEGMENT_SIZE), textTable(XML_TEXT_TABLE_SIZE_SMALL)
		{
			XMLCell& rootCell = cells.append(rootNode.value());
			cloneXMLNode(rootNode, rootCell);
		}
		void checkTableSize()
		{
			if (textTable.size() == XML_TEXT_TABLE_COUNT_THRESH)
			{
				Map<StringSpan, String> newTable(XML_TEXT_TABLE_SIZE_LARGE);
				for (auto pair = textTable.keyValues().firstElem(); pair.valid(); ++pair)
				{
					newTable[pair->key] = pair->value;
				}
				textTable = newTable;
			}
		}
		String touchText(StringSpan& key, StringSpan& val)
		{
			String output;
			if (textTable.tryGet(key, output)) return output;
			else
			{
				checkTableSize();
				String valString = val.clone();
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

using namespace spadas;
using namespace xml_internal;

const String spadas::XML::TypeName = "spadas.XML";
const String unknownTag = "unknown";

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
	if (readXMLBinary(span, xml)) return xml;
	else return Optional<XML>();
}

Optional<XML> XML::createFromBinary(Binary xmlBinary)
{
	XML xml;
	if (readXMLBinary(xmlBinary.sub(0), xml)) return xml;
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
		file.output(String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n").toBinary());
	}
	else
	{
		file.output(writeXMLNodeBinary(globalRoot(), 0));
	}
}

Binary XML::toBinary()
{
	if (!vars)
	{
		return String("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n").toBinary();
	}
	else
	{
		return writeXMLNodeBinary(globalRoot(), 0);
	}
}