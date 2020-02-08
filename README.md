# Graphics Programming
Repo for Graphics Programming college course where OpenGL is going to be taken as the main topic.

This just works on **Windows**, you will need to 

To compile and run, copy and paste the Makefile in the project root.
It will be needed to have the **make** command. To do this on Windows and MinGW is installed type this command:
```bash
copy %MINGW_PATH%\\bin\\mingw32-make.exe %MINGW_PATH%\\bin\\make.exe
```

Once the **.c** program is ready type:

```bash
make
make run
```

It is posible to **make and run** at the same time, but once the **.exe** is created if there are changes in the **.c** program it will be needed to compile without running.