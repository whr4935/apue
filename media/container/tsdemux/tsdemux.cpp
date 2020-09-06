#include <stdio.h>
#include <stdint.h>
#include <algorithm>
#include <libgen.h>
#include <string>
#include <fstream>
#include <string.h>
#include <map>
#include <assert.h>

#define MAX_SECTION_SIZE 4093
//#define MAX_PES_SIZE    65536

class TsParser {
public:
    enum SectionType {
        UNKNOWN_SECTION,
        PAT,
        PMT,
        CAT,
    };

    struct Section {
        Section(int pid, SectionType type) {
            this->pid = pid;
            this->type = type;
        }

        virtual ~Section() {
        }

        int pid = -1;
        SectionType type{UNKNOWN_SECTION};
        int sectionLength = 0;

        bool isComplete() const {
            if (size < 3) {
                return false;
            }

            int sectionLength = (mBuffer[1]&0x0F) << 8 | mBuffer[1];
            return this->size >= sectionLength + 3;
        }

        void clear() {
            sectionLength = 0;
            bufferIndex = 0;
            size = 0;
        }

        void append(const uint8_t* buffer, size_t _size) {
            memcpy(this->mBuffer+size, buffer, _size);
            size += _size;
        }

        bool empty() const {
            return this->size == 0;
        }

        const uint8_t* buffer() const {return mBuffer;}
        size_t bufferSize() const {return mBufferSize;}

        const uint8_t* data() {
            return mBuffer + bufferIndex;
        }
        size_t dataSize() const {
            return size;
        }
        const uint8_t* advance(int off) {
            bufferIndex += off;
            size -= off;

            return mBuffer + bufferIndex;
        }

    private:
        uint8_t mBuffer[MAX_SECTION_SIZE];
        int mBufferSize = MAX_SECTION_SIZE;

        int bufferIndex = 0;
        int size = 0; //current size
    };

    struct Stream {
        Stream(int pid, int type) {
            this->pid = pid;
            this->streamType = type;
        }

        ~Stream() {
            printf("stream:%d dtor!\n", pid);
            delete [] mBuffer;
        }

        int pid = -1;
        int streamType = -1;
        //int sectionLength = 0;

        int nextExpectedCounter = -1;
        bool started = false;

        void flush();
        void onPayloadData();

        bool isComplete() const {
            if (size < 6) {
                return false;
            }
            int packet_start_code_prefix = mBuffer[0]<<16 | mBuffer[1]<<8 | mBuffer[2];
            if (packet_start_code_prefix != 1) {
                return false;
            }
            int sectionLength = (mBuffer[4]&0x0F) << 8 | mBuffer[5];
            return sectionLength != 0 && size >= sectionLength + 6;
        }

        void clear() {
            bufferIndex = 0;
            size = 0;
        }

        void ensureBufferCapacity(int needSize) {
            if (mBuffer && mBufferSize > needSize) {
                return;
            }

            needSize = (needSize + 65535) & ~65535;
            uint8_t* newBuffer = new uint8_t[needSize];
            if (mBuffer) {
                memcpy(newBuffer, mBuffer, mBufferSize);
            }

            mBuffer = newBuffer;
            mBufferSize = needSize;
        }

        void append(const uint8_t* buffer, size_t _size) {
            memcpy(this->mBuffer+size, buffer, _size);
            size += _size;
        }

        bool empty() const {
            return this->size == 0;
        }

        const uint8_t* buffer() const {return mBuffer;}
        size_t bufferSize() const {return mBufferSize;}

        const uint8_t* data() {
            return mBuffer + bufferIndex;
        }
        size_t dataSize() const {
            return size;
        }
        const uint8_t* advance(int off) {
            bufferIndex += off;
            size -= off;

            return mBuffer + bufferIndex;
        }

    private:
        uint8_t* mBuffer = nullptr;
        int mBufferSize = 0;

        int bufferIndex = 0;
        int size = 0; //current size

        int mESCount = 0;
    };

    struct Program : Section {
        Program(int pid, int programNumber)
        : Section(pid, PMT) 
        {
            this->programNumber = programNumber;
        }

        ~Program() {
            printf("program dtor!\n");
        }

        void addStreamPid(int pid) {
            mStreamPids.push_back(pid);
        }

        int programNumber;
        int pcrPid = -1;

    private:
        std::vector<int> mStreamPids;
    };

    TsParser();
    ~TsParser();
    int parseTs(const uint8_t* buffer, int size);

private:
    int resync(const uint8_t* buffer, int size);
    void addSection(Section* section) {
        mSections.emplace(section->pid, section);
    }

    void addStream(Stream* stream) {
        mStreams.emplace(stream->pid, stream);
    }

    void parseSection(Section* section, const uint8_t* buffer, int size, int startIndicator, int continuityCounter);
    void parseDescriptor(Section* section);
    void parsePAT(Section* section);
    void parsePMT(Program* section);
    void parseCAT(Section* section);

    void parsePES(Stream* stream, const uint8_t* buffer, int size, int startIndicator, int continuityCounter);

private:
    std::map<int, Section*> mSections;
    std::map<int, Stream*> mStreams;

    TsParser(const TsParser&) = delete;
    TsParser& operator=(const TsParser&) = delete;
};

TsParser::TsParser()
{
    Section* section = new Section(0, PAT);
    addSection(section);

    section = new Section(1, CAT);
    addSection(section);
}

TsParser::~TsParser()
{
    printf("~TsParser!\n");
    for (auto& s : mSections) {
        delete s.second;
    }

    for (auto& s : mStreams) {
        delete s.second;
    }
}

int TsParser::parseTs(const uint8_t* buffer, int size)
{
    const uint8_t* p = buffer;
    int len = size;

    int skip = resync(buffer, size);
    if (skip > 0) {
        printf("skip %d bytes!\n", skip);
    }
    p += skip;
    size -= skip;

    while (size > 0) {
        len = std::min(size, 188);
        size -= len;

        if (len < 188) {
            break;
        }
        ++p;
        --len;

        int transport_error_indicator = (p[0]&0x80) >> 7;
        int payload_unit_start_indicator = (p[0]&0x40) >> 6;
        int pid = (p[0]<<8 | p[1]) & 0x1FFF;
        int transport_scrambling_control = (p[2]&0xC0) >> 6;
        int adaption_field_control = (p[2]&0x30) >> 4;
        int continuity_counter = p[2] & 0x0F;
        p += 3;
        len -= 3;
        if (adaption_field_control == 2 || adaption_field_control == 3) {
            int adaption_field_length = p[0];
            ++p;
            --len;

            int left_adaption_field_length = adaption_field_length;
            if (adaption_field_length > 0) {
                int pcr_flag = (p[0] & 0x10) >> 4;
                if (pcr_flag) {
                    uint64_t pcr = p[1] << 25 | p[2] << 17 | p[3] << 9 | p[4] << 1 | (p[5] & 0x80) >> 7;
                    pcr *= 300;
                    pcr += (p[5]&0x01) << 8 | p[6];
                    //printf("PCR:%lu us\n", pcr/27);

                    left_adaption_field_length -= 7;
                } else {
                    left_adaption_field_length -= 1;
                }
            }

            //printf("left adaption field length:%d\n", left_adaption_field_length);
            p += adaption_field_length;
            len -= adaption_field_length;
        }
        //printf("left payload size:%d\n", len);

        if (adaption_field_control == 1 || adaption_field_control == 3) {
            auto it = mSections.find(pid);
            if (it != mSections.end()) {
                Section* section = it->second;
                parseSection(section, p, len, payload_unit_start_indicator, continuity_counter);
            } else {
                auto itStream = mStreams.find(pid);
                if (itStream != mStreams.end()) {
                    Stream* stream = itStream->second;
                    parsePES(stream, p, len, payload_unit_start_indicator, continuity_counter);
                }
            }
        }

        p += len;
        len = 0;
    }

    return p - buffer;
}

int TsParser::resync(const uint8_t* buffer, int size)
{
    const uint8_t* p = buffer;

    while (size > 0) {
        if (*p != 0x47) {
            ++p;
            --size;
            continue;
        }

        if (size < 188) {
            break;
        }

        if (p[188] == 0x47) {
            break;
        }

        ++p;
        --size;
    }

    return p - buffer;
}

void TsParser::parseSection(Section* section, const uint8_t* buffer, int size, int startIndicator, int continuityCounter)
{
    const uint8_t* p = buffer;

    if (startIndicator) {
        if (!section->empty()) {
            section->clear();
        }

        int pointer_field = p[0];
        if (pointer_field > 0) {
            printf("pointer_field = %d\n", pointer_field);
        }
        p += pointer_field + 1;
        size -= pointer_field + 1;
    }

    section->append(p, size);
    if (!section->isComplete()) {
        return;
    }

    p = section->data();

    int table_id = p[0];
    int section_syntax_indicator = (p[1]&0x80) >> 7;
    assert(section_syntax_indicator == 1);
    int section_length = (p[1]&0x0F) << 4 | p[2];
    assert(section_length <= MAX_SECTION_SIZE);
    section->sectionLength = section_length;

#if 0
    int version_number = (p[5]&0x3E) >> 1;
    int current_next_indicator = (p[5]&0x01);
    int section_number = p[6];
    int last_section_number = p[7];
#endif

    p = section->advance(3);

    switch (section->type) {
    case PAT:
        parsePAT(section);
        break;

    case PMT:
        {
            Program* program = static_cast<Program*>(section);
            parsePMT(program);
        }
        break;

    case CAT:
        parseCAT(section);
        break;

    default:
        printf("unknown section!\n");
        break;
    }

    section->clear();
}

void TsParser::parseDescriptor(Section* section)
{

}

void TsParser::parsePAT(Section* section)
{
    //printf("%s:%d\n", __FUNCTION__, __LINE__);

    const uint8_t* p = section->data();

    int transport_stream_id = p[0]<<8 | p[1];

    //skip section header
    p = section->advance(5);
    int numPrograms = (section->sectionLength - 5 - 4) / 4;
    for (int i = 0; i < numPrograms; ++i) {
        int program_number = p[0]<<4 | p[1];
        //printf("program_number = %d\n", program_number);

        if (program_number != 0) {
            int program_map_PID = (p[2]&0x01F) << 8 | p[3];

            auto it = mSections.find(program_map_PID);
            if (it == mSections.end()) {
                printf("programNumber:%d, program_map_PID = %d\n", program_number, program_map_PID);
                Program *program= new Program(program_map_PID, program_number);
                addSection(program);
            }
        } else {
            int network_PID = (p[2]&0x1F) << 8 | p[3];
        }

        p = section->advance(4);
    }

    int32_t crc = p[0]<<24 | p[1]<<16 | p[2]<<8 | p[4];
}

void TsParser::parsePMT(Program* program)
{
    const uint8_t* p = program->data();

    int programNumber = p[0] << 8 | p[1];
    //printf("programNumber:%d\n", programNumber);

    //skip section header
    p = program->advance(5);

    int pcr_pid = (p[0]&0x1F)<<8 | p[1];
    program->pcrPid = pcr_pid;
    int program_info_length = (p[2]&0x0F) << 8 | p[3];
    assert(program_info_length < 1024);
    p = program->advance(4);

    if (program_info_length > 0) {
        int descriptorsRemaining = program_info_length;
        const uint8_t* p2 = p;
        int count = 0;
        while (descriptorsRemaining >= 2) {
            int descriptor_tag = p2[0];
            int descriptor_length = p2[1];
            switch (descriptor_tag) {
            case 0x09:
            {
                int ca_system_id = p2[2]<<8 | p2[3]; 
                int ecm_pid = (p2[4]&0x1F)<<8 | p2[4];
                printf("ca_system_id:%#x, ecm_pid:%#x, count:%d, descriptor_length:%d\n", ca_system_id, ecm_pid, ++count, descriptor_length);
            }
            break;

            default:
                break;
            }

            p2 += 2 + descriptor_length;
            descriptorsRemaining -= 2 + descriptor_length;
        }
    }

    //skip program info
    p = program->advance(program_info_length);

    int infoBytesRemaining = program->sectionLength - 9 - program_info_length - 4;
    while (infoBytesRemaining >= 5) {
        int stream_type = p[0];
        int elementary_pid = (p[1]&0x1F) << 8 | p[2];
        int es_info_length = (p[3]&0x0F) << 8 | p[4];
        p = program->advance(5);
        infoBytesRemaining -= 5;

        if (es_info_length > 0) {
            int descriptorsRemaining = es_info_length;
            const uint8_t* p2 = p;
            int count = 0;
            while (descriptorsRemaining >= 2) {
                int descriptor_tag = p2[0];
                int descriptor_length = p2[1];
                switch (descriptor_tag) {
                case 0x09:
                {
                    int ca_system_id = p2[2]<<8 | p2[3]; 
                    int ecm_pid = (p2[4]&0x1F)<<8 | p2[4];
                    printf("streamType:%#x, pid:%d, ca_system_id:%#x, ecm_pid:%#x, count:%d, descriptor_length:%d\n",
                            stream_type, elementary_pid, ca_system_id, ecm_pid, ++count, descriptor_length);
                }
                break;

                default:
                    break;
                }

                p2 += 2 + descriptor_length;
                descriptorsRemaining -= 2 + descriptor_length;
            }

            p = program->advance(es_info_length);
            infoBytesRemaining -= es_info_length;
        }

        auto it = mStreams.find(elementary_pid);
        if (it == mStreams.end()) {
            printf("programNumber:%d, stream pid:%d, type:%#x\n", programNumber, elementary_pid, stream_type);
            Stream* stream = new Stream(elementary_pid, stream_type);
            addStream(stream);

            program->addStreamPid(elementary_pid);
        }
    }

    int32_t crc = p[0]<<24 | p[1]<<16 | p[2]<<8 | p[4];
}

void TsParser::parseCAT(Section* section)
{
    const uint8_t* p = section->data();

    //skip section header
    p = section->advance(5);

    int descriptorsRemaining = section->sectionLength - 5 - 4;
    int count = 0;
    while (descriptorsRemaining >= 2) {
        int descriptor_tag = p[0];
        int descriptor_length = p[1];
        switch (descriptor_tag) {
        case 0x09:
        {
            int ca_system_id = p[2]<<8 | p[3]; 
            int emm_pid = (p[4]&0x1F)<<8 | p[4];
            //printf("ca_system_id:%#x, emm_pid:%#x, count:%d, descriptor_length:%d\n", ca_system_id, emm_pid, ++count, descriptor_length);
        }
        break;

        default:
            break;
        }

        p = section->advance(2 + descriptor_length);
        descriptorsRemaining -= 2 + descriptor_length;
    }

    if (descriptorsRemaining != 0) {
        printf("descriptorsRemaining = %d\n", descriptorsRemaining);
    }

    int32_t crc = p[0]<<24 | p[1]<<16 | p[2]<<8 | p[4];
}

void TsParser::parsePES(Stream* stream, const uint8_t* buffer, int size, int startIndicator, int continuityCounter)
{
    if (stream->nextExpectedCounter != -1 && stream->nextExpectedCounter != continuityCounter) {
        printf("stream:%d discontinue! nextExpectedCounter:%d, continuityCounter:%d\n", stream->pid,
                stream->nextExpectedCounter, continuityCounter);

        stream->started = false;
        stream->clear();
    }
    stream->nextExpectedCounter = continuityCounter+1 & 0x0F;

    if (startIndicator) {
        if (stream->started) {
            stream->flush();
        }

        stream->started = true;
    }

    if (!stream->started) {
        return;
    }

    int needSize = stream->dataSize() + size;
    stream->ensureBufferCapacity(needSize);
    stream->append(buffer, size);
    if (!stream->isComplete()) {
        return;
    }

    stream->flush();
    stream->started = false;
}

void TsParser::Stream::flush()
{
    const uint8_t* p = data();
    int packet_start_code_prefix = p[0]<<16 | p[1]<<8 | p[2];
    if (packet_start_code_prefix != 1) {
        printf("streamType:%#x, pid:%d, packet_start_code_prefix=%#x, mESCount:%d, size:%d\n", streamType, pid, packet_start_code_prefix, mESCount, size);
        return;
    }
    //assert(packet_start_code_prefix == 1);
    int stream_id = p[3];
    int pes_packet_length = p[4]<<8 | p[5];
    if (pes_packet_length == 0) {
        //printf("streamTy = %d, pes_packet_length = 0\n", streamType);
    }

    p = advance(6);

    if (stream_id != 0x1bc && stream_id != 0x1bf && /* program_stream_map, private_stream_2 */
        stream_id != 0x1f0 && stream_id != 0x1f1 && /* ECM, EMM */
        stream_id != 0x1ff && stream_id != 0x1f2 && /* program_stream_directory, DSMCC_stream */
        stream_id != 0x1f8) {                  /* ITU-T Rec. H.222.1 type E stream */
        assert((p[0]&0xC0) == 0x80);
        int pes_scrambling_control = (p[0]&0x30) >> 6;
        int pes_priority = (p[0]&0x80) >> 3;
        int pts_dts_flag = (p[1]&0xC0) >> 6;
        int escr_flag = (p[1]&0x20) >> 5;
        int es_rate_flag = (p[1]&0x10) >> 4;
        int dsm_trick_mode_flag = (p[1]&0x08) >> 3;
        int additional_copy_info_flag = (p[1]&0x04) >> 2;
        int pes_crc_flag = (p[1]&0x02) >> 1;
        int pes_extension_flag = p[1]&0x01;
        int pes_header_data_length = p[2];
        int optional_bytes_remaining = pes_header_data_length;
        p = advance(3);

        int64_t pts = 0;
        int64_t dts = 0;
        if (pts_dts_flag == 2 || pts_dts_flag == 3) {
            pts = (p[0]&0x0E) << 29 |
                p[1] << 23 |
                (p[2]&0xFE) << 14 |
                p[3] << 7 |
                p[4]>>1;
            p = advance(5);
            optional_bytes_remaining -= 5;
        }

        if (pts_dts_flag == 3) {
            dts = (p[0]&0x0E) << 29 |
                p[1] << 23 |
                (p[2]&0xFE) << 14 |
                p[3] << 7 |
                p[4]>>1;
            p = advance(5);
            optional_bytes_remaining -= 5;
        }

        assert(optional_bytes_remaining >= 0);
        p = advance(optional_bytes_remaining);
        if (pes_packet_length != 0) {
            int dataLength = pes_packet_length - 3 - pes_header_data_length;
        } else {
            
        }
    }

    clear();
    ++mESCount;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    //std::string filename = argv[1];
    std::string filename = "/mnt/f/assets/scte27.ts";

    if (argc > 2) {
        printf("Usage: %s filename\n", basename(argv[0]));
        return 0;
    } else if (argc == 2) {
        filename = argv[1];
    }

    std::fstream f(filename);
    printf("is_open:%d\n", f.is_open()); 

    if (!f.is_open()) {
        return -1;
    }

    TsParser parser;

    int count = 0;
    
    uint8_t buffer[1024];
    int gcount = 0;
    int size;
    int off = 0;
    int len = 0;
    while (f) {
        f.read((char*)buffer+off, sizeof(buffer)-off);
        gcount = f.gcount();
        //printf("gcount:%d\n", f.gcount());

        size = off + gcount;
        if (size != 1024) {
            //printf("last size:%d, off = %d, gcount = %d\n", size, off, gcount);
        }

        //printf("count = %d\n", ++count);
        len = parser.parseTs(buffer, size);
        off = size-len;
        memmove(buffer, buffer + len, off);
    }

    printf("finished! %d %d %d %d\n", f.eof(), f.fail(), f.bad(), f.good());






    return 0;
}
