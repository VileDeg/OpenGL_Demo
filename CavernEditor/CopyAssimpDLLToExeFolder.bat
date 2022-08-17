set lib=%1
set tdir=%2
set tdir=%tdir:/=\%
echo n | copy /-y ..\Crave\vendor\assimp\lib\%lib% %tdir%
PAUSE