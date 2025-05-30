SET ResX=800
SET ResY=600
SET V_GAME_MAP=/Game/RS/Maps/Lobby_Map
SET Start_Mode=Trainee
SET DeviceName=VR-04
SET DeviceId=
call XRTraining.exe %V_GAME_MAP% -StartMode=%Start_Mode% -DeviceName=%DeviceName% -DeviceId=%DeviceId% -WINDOWED ResX=%ResX% ResY=%ResY%

