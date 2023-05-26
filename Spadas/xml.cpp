
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
			tag = bracketContent.subString(0, spaceLocations[0]);
			attributesString = bracketContent.subString(spaceLocations[0]+1);
		}
		
		if (tag.isEmpty()) return FALSE;
		else return TRUE;
	}

	Array<XMLAttribute> unpackAttributes(String attributesString)
	{
		Array<StringSpan> subStrings = attributesString.split("\"");

		Array<XMLAttribute> out(subStrings.size() / 2);
		for (UInt i = 0; i < out.size(); i++)
		{
			XMLAttribute& attribute = out[i];
			String nameString = subStrings[2 * i];
			String valueString = subStrings[2 * i + 1];

			if (!nameString.isEmpty()) continue;

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

			attribute.name = buffer;
			attribute.value = decodeES(valueString);
		}
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
	
	Bool readXMLBinary(Binary xmlBinary, XMLNode& out)
	{
		String rawString(xmlBinary);
		
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
					bracketContents[i] = rawString.subString(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-1);
					break;
					
				case BracketType::End:
					bracketContents[i] = rawString.subString(leftAngleLocations[i]+2, rightAngleLocations[i]-leftAngleLocations[i]-2);
					break;
					
				case BracketType::Atom:
					bracketContents[i] = rawString.subString(leftAngleLocations[i]+1, rightAngleLocations[i]-leftAngleLocations[i]-2);
					break;
					
				default:
					break;
			}
			
			/* get external content */
			if (i != nBrackets - 1)
			{
				externalContents[i] = decodeES(rawString.subString(rightAngleLocations[i]+1, leftAngleLocations[i+1]-rightAngleLocations[i]-1));
			}
		}
		
		SPADAS_ERROR_RETURNVAL(startBracketNum != endBracketNum, FALSE);
		
		/* generate an XML tree */
		XMLNode root, current = root;

		for (UInt i = 0; i < nBrackets; i++)
		{
			if (bracketTypes[i] == BracketType::Dummy) continue;
			
			if (bracketTypes[i] == BracketType::Start)
			{
				String tag, attributesString;
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], tag, attributesString), FALSE);

				current = current.addLeaf(XMLElement(tag, unpackAttributes(attributesString), externalContents[i]));
			}

			if (bracketTypes[i] == BracketType::End)
			{
				SPADAS_ERROR_RETURNVAL(bracketContents[i] != current.value().tag, FALSE);
				
				XMLNode upNode = current.root();
				SPADAS_ERROR_RETURNVAL(upNode == current, FALSE);

				current = upNode;
			}
			
			if (bracketTypes[i] == BracketType::Atom)
			{
				String tag, attributesString;
				SPADAS_ERROR_RETURNVAL(!extractTag(bracketContents[i], tag, attributesString), FALSE);

				current.addLeaf(XMLElement(tag, unpackAttributes(attributesString), String()));
			}
		}
		
		SPADAS_ERROR_RETURNVAL(root.nLeaves() != 1, FALSE);

		out = root.leafAt(0);
		out.cutRoot();
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
			
			out = outString.toBinary();
		}
		else
		{
			UInt size = 0, index = 0;
			
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
	class XMLVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(XML, Vars)

		XMLNode globalRoot;
        ~XMLVars()
        {
            globalRoot.collapse();
        }
	};
}

using namespace spadas;
using namespace xml_internal;

const String spadas::XML::TypeName = "spadas.XML";

XMLAttribute::XMLAttribute()
{
}

XMLAttribute::XMLAttribute(String name0, String value0) : name(name0), value(value0)
{
}

XMLElement::XMLElement() : tag("unknown")
{
}
XMLElement::XMLElement(String tag0) : tag(tag0.isEmpty() ? "unknown" : tag0)
{
}
XMLElement::XMLElement(String tag0, Array<XMLAttribute> attributes0, String content0) : tag(tag0.isEmpty() ? "unknown" : tag0), attributes(attributes0), content(content0)
{
}

XML::XML()
{
}

XML::XML(XMLNode xmlRoot) : Object<class XMLVars>(new XMLVars(), TRUE)
{
	vars->globalRoot = xmlRoot.cloneTree();
	if (vars->globalRoot.value().tag.isEmpty()) vars->globalRoot.value().tag = "root";
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

    if (binary[0] == 0xEF && binary[1] == 0xBB && binary[2] == 0xBF)
    {
        Binary tmp(binary.size()-3);
        utility::memoryCopy(&binary[3], tmp.data(), binary.size()-3);
        binary = tmp;
    }
	else
	{
		SPADAS_WARNING_MSG("No BOM");
	}

	return createFromBinary(binary);
}

Optional<XML> XML::createFromBinary(Binary xmlBinary)
{
	XMLNode globalRoot;
	Bool ret = readXMLBinary(xmlBinary, globalRoot);
	if (!ret) return Optional<XML>();
	
	if (globalRoot.value().tag.isEmpty()) globalRoot.value().tag = "root";
	
	XML xml;
	xml.setVars(new XMLVars(), TRUE);
	xml.vars->globalRoot = globalRoot;
	return xml;
}

XML XML::clone()
{
	if (!vars) return XML();
    else return XML(vars->globalRoot);
}

XMLNode XML::globalRoot()
{
	if (!vars)
	{
		setVars(new XMLVars(), TRUE);
		vars->globalRoot->tag = "root";
	}
	return vars->globalRoot;
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
		String xmlString = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n";
		file.output(xmlString.toBinary());
	}
	else
	{
		String header = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		file.output(header.toBinary());

		Binary xmlBinary = writeXMLNodeBinary(vars->globalRoot, 0);
		file.output(xmlBinary);
	}
}

Binary XML::toBinary()
{
	if (!vars)
	{
		String xmlString = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n";
		return xmlString.toBinary();
	}
	else
	{
		String header = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
		return header.toBinary() + writeXMLNodeBinary(vars->globalRoot, 0);
	}
}

Array<XMLNode> XML::nodeLeavesWithTagName(XMLNode node, String tagName)
{
	SPADAS_ERROR_RETURNVAL(tagName.isEmpty(), Array<XMLNode>());

	UInt nLeaves = node.nLeaves();
	Array<XMLNode> leaves = node.leaves();
	Array<UInt> okIndices(nLeaves);
	UInt okNum = 0;
	for (UInt i = 0; i < leaves.size(); i++)
	{
		if (leaves[i].value().tag == tagName) okIndices[okNum++] = i;
	}
	Array<XMLNode> out = Array<XMLNode>::createUninitialized(okNum);
	for (UInt i = 0; i < okNum; i++)
	{
		out.initialize(i, leaves[okIndices[i]]);
	}
	return out;
}

Bool XML::firstNodeLeafWithTagName(XMLNode node, String tagName, XMLNode& output)
{
	SPADAS_ERROR_RETURNVAL(tagName.isEmpty(), FALSE);

	UInt nLeaves = node.nLeaves();
	if (nLeaves == 0) return FALSE;

	Array<XMLNode> leaves = node.leaves();
	for (UInt i = 0; i < nLeaves; i++)
	{
		if (leaves[i].value().tag != tagName) continue;

		output = leaves[i];
		return TRUE;
	}

	return FALSE;
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
