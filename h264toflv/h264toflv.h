#define FLV_TAG_HEAD_LEN 11
#define FLV_PRE_TAG_LEN 4

void write_sps_pps()
{
    int32_t dts;
    uint8_t* sps;
    int32_t spsLen;
    uint8_t* pps;
    int32_t ppsLen;
    int32_t offset = 0;

    int32_t bodyLen = spsLen + ppsLen + 16;
    int32_t outpuLen = bodyLen + FLV_PRE_TAG_LEN;

    uint8_t* output = new uint8_t[outpuLen];

    output[offset++] = 0x09;
    output[offset++] = (uint8_t)(bodyLen >> 16);
    output[offset++] = (uint8_t)(bodyLen >> 8);
    output[offset++] = (uint8_t)(bodyLen);

    output[offset++] = (uint8_t)(dts >> 16);
    output[offset++] = (uint8_t)(dts >> 8);
    output[offset++] = (uint8_t)(dts);
    output[offset++] = (uint8_t)(dts >> 24);

    output[offset++] = 0x00;
    output[offset++] = 0x00;
    output[offset++] = 0x00;

    //flv video tag header
    output[offset++] = 0x17;
    output[offset++] = 0x00;
    output[offset++] = 0x00;
    output[offset++] = 0x00;
    output[offset++] = 0x00;

    //flv video body --AVCDecoderConfigurationRecord
    output[offset++] = 0x01;
    output[offset++] = sps[1];
    output[offset++] = sps[2];
    output[offset++] = sps[3];
    output[offset++] = 0xff;
    output[offset++] = 0xe1;
    output[offset++] = (uint8_t)(spsLen >> 8);
    output[offset++] = (uint8_t)(spsLen);
    memcpy(output + offset, sps, spsLen);
    offset += spsLen;
    output[offset++] = 0x01;
    output[offset++] = (uint8_t)(ppsLen >> 8);
    output[offset++] = (uint8_t)(ppsLen);
    memcpy(output + offset, pps, ppsLen);
    offset += ppsLen;

    //prev tag size, rtmp no need
    int32_t prevTagLen = bodyLen + FLV_TAG_HEAD_LEN;
    output[offset++] = (uint8_t)(prevTagLen >> 24);
    output[offset++] = (uint8_t)(prevTagLen >> 16);
    output[offset++] = (uint8_t)(prevTagLen >> 8);
    output[offset++] = (uint8_t)(prevTagLen);

    dts += 33;
}

void write_nal(const uint8_t* nal, int32_t nalLen)
{
    int32_t dts;
    int32_t offset = 0;
    
    nal += 4 //00 00 00 01 startup flag
    nalLen -= 4;

    uint8_t isKeyFrame = 0;
    isKeyFrame = ((nal[0] & 0x1f) == 0x05) ? 0x17 : 0x27;

    int32_t bodyLen = nalLen + 9;
    int32_t outpuLen = bodyLen + FLV_TAG_HEAD_LEN + FLV_PRE_TAG_LEN;
    uint8_t* output = new uint8_t[outpuLen];

    //flv tag header
    output[offset++] = 0x09; //tag type video
    output[offset++] = (uint8_t)(bodyLen >> 16);
    output[offset++] = (uint8_t)(bodyLen >> 8);
    output[offset++] = (uint8_t)(bodyLen);

    output[offset++] = (uint8_t)(dts >> 16); //timestamp
    output[offset++] = (uint8_t)(dts >> 8);
    output[offset++] = (uint8_t)(dts);
    output[offset++] = (uint8_t)(dts >> 24);

    output[offset++] = 0x00; //stream id 0
    output[offset++] = 0x00;
    output[offset++] = 0x00;

    //flv video tag header
    output[offset++] = isKeyFrame; //key frame, AVC
    output[offset++] = 0x01; //avc NALU unit

    int32_t cts = 80;   //need comosit
    output[offset++] = (uint8_t)(cts >> 16);    //composit time
    output[offset++] = (uint8_t)(cts >> 8);
    output[offset++] = (uint8_t)(cts);

    output[offset++] = (uint8_t)(nalLen >> 24);
    output[offset++] = (uint8_t)(nalLen >> 16);
    output[offset++] = (uint8_t)(nalLen >> 8);
    output[offset++] = (uint8_t)(nalLen);
    memcpy(output + offset, nal, nalLen);
    offset += nalLen;

    //prev tag size, rtmp no need
    int32_t prevTagLen = bodyLen + FLV_TAG_HEAD_LEN;
    output[offset++] = (uint8_t)(prevTagLen >> 24);
    output[offset++] = (uint8_t)(prevTagLen >> 16);
    output[offset++] = (uint8_t)(prevTagLen >> 8);
    output[offset++] = (uint8_t)(prevTagLen);

    dts += 33;
}