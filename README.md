# spur-gear-gen
CAD wrapped spur gear generator

To compile requires Visual Studio 2015 and MFC. However, all of the CAD work is done with boost, and with no Microsoft dependence, so should cross platform easily. This is version 0.1 Achievements for 1.0 status would be a reasonable user interface and an installer package.

Jan 2, 2017
Read the how_to_build.txt in the project folder. It should all be complete and there should be no dependences on my machine

Jan 4, 2017
I have incorporated the 'xml settings store'. there is a settings.xml in the appdata folder that will show the gear dialog. I noticed that when you open the dxf file project it may revert to x64. Look for that and change it back to x86 or it will not create the proper lib and put it in the right place. To save your dxf file of the gear, use file->test and it will get written as test2.dxf. Just one gear is written even though two are shown in the view. It is almost time to get a proper file system together and see about adding real CAD editing to this thing.
