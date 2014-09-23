#ifndef serial_protocol_h
#define serial_protocol_h

// use lower case so its safe to use ALL CAPS for debug messages
const char COMMAND_DELIMITER_BREATH = 'b';
const char COMMAND_DELIMITER_PANIC = 'p';
const char COMMAND_DELIMITER_SCENE = 's';
const char COMMAND_DELIMITER_CUE = 'c';
const char COMMAND_DELIMITER_PARAMETER = 'v';
const char COMMAND_DELIMITER_END = '\n';

#endif