
//Gets the next state of the LFSR by stepping eight times.
unsigned int nextState(unsigned int F, unsigned int S) {
    for (int i = 0; i < 8; i++) {
        if ((S & 0x00000001) == 0x00000000) {
            S = S >> 1;
        }
        else {
            S = (S >> 1) ^ F;
        }
    }
    return S;
}

//Crypts or decrypts <data> using an LFSR
unsigned char* Crypt(unsigned char* data, int dataLength, unsigned int initialValue, unsigned int feedback) {
    unsigned int& currentState = initialValue;
    unsigned char* result = nullptr;
    result = new unsigned char[dataLength];
    for (int i = 0; i < dataLength; i++) {
        currentState = nextState(feedback, currentState);
        unsigned char lastbyte = 0x000000FF & currentState;
        result[i] = data[i] ^ lastbyte;
    }

    return result;
}


unsigned char* Crypt(unsigned char* data, int dataLength, unsigned int initialValue) {
    return Crypt(data, dataLength, initialValue, 0x87654321);
}
