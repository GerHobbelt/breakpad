#pragma once

#include <string>
#include <vector>
#include <ostream>

class CBinarySymbolsWriter
{
	private:

		struct CHeader
		{
			uint32_t m_Version;

			uint32_t m_OSOffset;
			uint32_t m_ArchOffset;
			uint32_t m_UUIDOffset;
			uint32_t m_NameOffset;

			uint32_t m_FunctionsOffset;
			uint32_t m_nFunctions;

			uint32_t m_LinesOffset;
			uint32_t m_nLines;

			uint32_t m_StringDataOffset;
		};

		struct CLinesHeader
		{
			uint32_t m_nLines;
		};

		struct CLine
		{
			uint64_t m_Address;
			uint64_t m_Size;

			uint32_t m_Line;
			uint32_t m_File;
		}; // 24 bytes (in file)

		struct CFunction
		{
			uint64_t m_Address;
			uint64_t m_Size;
			uint64_t m_StackParamSize;

			uint32_t m_NameOffset;

			uint32_t m_FirstLine;

		}; // 32 bytes (in file)

		enum
		{
			BytesPerFunction = 32,
			BytesPerLine = 24,
			BytesPerFunctionLines = 4,
		};

	private:
		std::vector<char> mp_lStringData;

		uint32_t fp_InsertString(std::string const& _String)
		{
			size_t nStringBytes = _String.size();
			size_t nTotalBytes = nStringBytes + sizeof(uint32_t);

			size_t iOffset = mp_lStringData.size();
			mp_lStringData.resize(iOffset + nTotalBytes);

			uint32_t StrLen = nStringBytes;
			memcpy(&mp_lStringData[iOffset], &StrLen, sizeof(StrLen));
			memcpy(&mp_lStringData[iOffset + sizeof(StrLen)], _String.c_str(), nStringBytes);

			return (uint32_t)iOffset;
		}

		CHeader mp_Header;
		std::ostream& mp_Stream;
		uint32_t mp_LineOffset;
		std::map<uint32_t, uint32_t> mp_FileNameLookup;

	public:

		CBinarySymbolsWriter(	std::ostream& _Stream
							,	char const* _pOS
							,	char const* _pArch
							,	char const* _pUUID
							,	char const* _pName
							,	size_t _nFiles
							,	size_t _nFunctions
							,	size_t _nLines)
			: mp_Stream(_Stream)
			, mp_LineOffset(0)
		{
			memset(&mp_Header, 0, sizeof(mp_Header));

			mp_Header.m_OSOffset = fp_InsertString(_pOS);
			mp_Header.m_ArchOffset = fp_InsertString(_pArch);
			mp_Header.m_UUIDOffset = fp_InsertString(_pUUID);
			mp_Header.m_NameOffset = fp_InsertString(_pName);
			
			mp_Header.m_nFunctions = _nFunctions;
			mp_Header.m_nLines = _nLines;

			mp_Header.m_FunctionsOffset = sizeof(CHeader);
			mp_Header.m_LinesOffset = mp_Header.m_FunctionsOffset + mp_Header.m_nFunctions * BytesPerFunction;
			mp_Header.m_StringDataOffset = 		mp_Header.m_LinesOffset
											+	mp_Header.m_nLines * BytesPerLine
											+	mp_Header.m_nFunctions * BytesPerFunctionLines;

			//mp_Header.m_FunctionsOffset + mp_Header.m_nFunctions * BytesPerFunction;
			
			mp_LineOffset = mp_Header.m_LinesOffset;

			mp_Stream.write( (char const*)&mp_Header, sizeof(mp_Header));
		}

		~CBinarySymbolsWriter()
		{
		}

		void f_AddFile(uint32_t _Index, std::string const& _Filename)
		{
			uint32_t NameOffset = fp_InsertString(_Filename);

			mp_FileNameLookup[_Index] = NameOffset;
		}

		void f_AddFunction(uint64_t _Address, std::vector<google_breakpad::Module::Range> const &_Ranges, uint64_t _StackParamSize, std::string const& _Name, uint32_t _nLines)
		{
			mp_Stream.write( (char const*)&_Address, sizeof(_Address));
			uint32_t nRanges = _Ranges.size();
			uint64_t HighestRange = _Address;
			mp_Stream.write( (char const*)&nRanges, sizeof(nRanges));
			for (auto &Range : _Ranges)
			{
				uint64_t EndRange = Range.address + Range.size;
				if (EndRange > HighestRange)
					HighestRange = EndRange;
			}
			mp_Stream.write((char const*)&HighestRange, sizeof(HighestRange));

			mp_Stream.write( (char const*)&_StackParamSize, sizeof(_StackParamSize));
			uint32_t NameOffset = fp_InsertString(_Name);
			mp_Stream.write( (char const*)&NameOffset, sizeof(NameOffset));

			uint32_t FirstLine = mp_LineOffset;
			mp_Stream.write( (char const*)&FirstLine, sizeof(FirstLine));

			mp_LineOffset += BytesPerFunctionLines + _nLines * BytesPerLine;
		}

		void f_BeginFunctionLines(uint32_t _nLines)
		{
			mp_Stream.write( (char const*)&_nLines, sizeof(_nLines));
		}

		void f_AddFunctionLine(uint64_t _Address, uint64_t _Size, uint32_t _Line, uint32_t _File)
		{
			mp_Stream.write( (char const*)&_Address, sizeof(_Address));
			mp_Stream.write( (char const*)&_Size, sizeof(_Size));
			mp_Stream.write( (char const*)&_Line, sizeof(_Line));

			uint32_t FileOffset = ~(uint32_t)0;

			std::map<uint32_t, uint32_t>::const_iterator FIter = mp_FileNameLookup.find(_File);
			if (FIter != mp_FileNameLookup.end())
				FileOffset = (*FIter).second;

			mp_Stream.write( (char const*)&FileOffset, sizeof(FileOffset));
		}

		void f_Finish()
		{
			mp_Stream.write( (char const*)&mp_lStringData[0], mp_lStringData.size());
			mp_lStringData.clear();
			mp_LineOffset = 0;
		}
};
