# Modbus library that works with software serial.

This is a copy of the library at https://github.com/angeloc/simplemodbusng. The buffer size is reduced from 128 bytes to 64 bytes to make sure
that the Attiny85 does not run out of memmory.