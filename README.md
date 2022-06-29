# bugcheckdriver
A driver that allows the user to crash the system and specify the BugCheckCode.

Create a service entry for the driver by typing sc create bugcheckdriver type= kernel binPath= X:\bugcheckdriver.sys into a command prompt.

Start the driver by typing sc start bugcheckdriver. In most circumstances that will require the disabling of driver signature enforcement or enabling test signing.

Use the client application to specify a BugCheckCode and crash your system.

The project files are VC++ 2008 compatible.
