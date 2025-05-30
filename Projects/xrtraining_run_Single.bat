SET ResX=1600	
SET ResY=1350
SET V_GAME_MAP=/Game/RS/Maps/Lobby_Map
SET Start_Mode=Single
SET DeviceName=VR-05
SET DeviceId=
SET InputMode=2
SET RecordSaveMode=0
SET WeaponType=1
call XRTraining.exe %V_GAME_MAP% -StartMode=%Start_Mode% -DeviceName=%DeviceName% -DeviceId=%DeviceId% -WINDOWED ResX=%ResX% ResY=%ResY% -InputMode=%InputMode% -RecordSaveMode=%RecordSaveMode% -WeaponType=%WeaponType%

