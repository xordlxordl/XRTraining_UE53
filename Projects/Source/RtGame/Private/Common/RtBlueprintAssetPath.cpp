// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/RtBlueprintAssetPath.h"

namespace RtBlueprintAsset
{
    const TCHAR* MainCharacter = TEXT("/Game/RS/Training/Blueprints/BP_RtTraineePawn");
	const TCHAR* ControlCharacter = TEXT("/Game/RS/Control/Blueprints/BP_RcCharacter");
	const TCHAR* ControlHUD = TEXT("/Game/RS/Control/Widgets/WBP_RcControlHUD");
	const TCHAR* TraineeSkeletalMesh = TEXT("/Game/CommonModels/SkeletalModels/Models/Default_Character/Mesh/SK_Mannequin.SK_Mannequin");
	const TCHAR* TraineeUTAnimClass = TEXT("/Game/RS/Training/Blueprints/ABP_TraineeAnim_UT.ABP_TraineeAnim_UT_C");
	const TCHAR* ControlMode_FollowCamera = TEXT("/Game/RS/Control/Camera/BP_Ctrl_FollowCamera.BP_Ctrl_FollowCamera_C");
	const TCHAR* LoadingScreenWidget = TEXT("/Game/RS/Common/Widgets/LoadingWidget.LoadingWidget_C");
	const TCHAR* LoadingPopupWidget = TEXT("/Game/RS/Common/Widgets/LoadingPopup.LoadingPopup_C");
	const TCHAR* RtClone = TEXT("/Game/RS/Training/Blueprints/BP_RtClone.BP_RtClone_C");
	const TCHAR* RtVRMeshes = TEXT("/Game/RS/Training/Blueprints/BP_RtVRMeshes.BP_RtVRMeshes_C");
	const TCHAR* RtStartAreaEffect = TEXT("/Game/RS/Training/Blueprints/BP_StartAreaEffect.BP_StartAreaEffect_C");
	const TCHAR* RtReplayTrainee = TEXT("/Game/RS/Replay/ReplayActor/BP_RtReplayTrainee.BP_RtReplayTrainee_C");
	const TCHAR* RtReplayTraineeAnim = TEXT("/Game/RS/Replay/ReplayActor/ABP_RtReplayTrainee.ABP_RtReplayTrainee_C");
	const TCHAR* DangerAlertSound = TEXT("/Game/RS/Training/Blueprints/Safety/Alert.Alert");
	const TCHAR* GuideHand = TEXT("/Game/RS/Training/Blueprints/Hands/BP_GuideHand.BP_GuideHand_C");
	const TCHAR* ListenIcon = TEXT("/Game/RS/Training/Blueprints/Widgets/BP_ListenIconActor.BP_ListenIconActor_C");
	const TCHAR* DirectionArrowMesh = TEXT("/Game/CommonModels/StaticModels/Icon_Arrow.Icon_Arrow");
}

// 관제 UI
namespace RtControlWidgetBlueprint
{
	const TCHAR* ControlLoginWidget = TEXT("/Game/RS/Control/Widgets/WBP_RcLogin.WBP_RcLogin_C");
    const TCHAR* ControlSelectWidget = TEXT("/Game/RS/Control/Widgets/WBP_SelectMenu.WBP_SelectMenu_C");
	const TCHAR* ControlSelectMapBlock = TEXT("/Game/RS/Control/Widgets/Parts/Block/WBP_Map_Block.WBP_Map_Block_C");
	const TCHAR* ControlAccidentBlock = TEXT("/Game/RS/Control/Widgets/Parts/Block/WBP_Accident_Block.WBP_Accident_Block_C");
	const TCHAR* ControlAssignedBlock = TEXT("/Game/RS/Control/Widgets/Parts/Block/WBP_Trainee_Block20.WBP_Trainee_Block20_C");
}

// 훈련자 UI
namespace RtTraineeWidgetBlueprint
{
	const TCHAR* RtWidgetActor = TEXT("/Game/Rs/Common/Blueprints/BP_RtWidgetActor.BP_RtWidgetActor_C");
	const TCHAR* RtTraineeResultWidgetActor = TEXT("/Game/RS/Training/Blueprints/Widgets/BP_TraineeResultActor.BP_TraineeResultActor_C");
	const TCHAR* TraineeMainWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_TraineeMain.WBP_TraineeMain_C");
	const TCHAR* TraineeTextWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trainee_Text.WBP_Trainee_Text_C");
	const TCHAR* TraineeMenuOption = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trainee_MenuOption.WBP_Trainee_MenuOption_C");
	const TCHAR* TraineeSelectionWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trainee_Selection.WBP_Trainee_Selection_C");
	const TCHAR* TraineeSelectionMenuWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trainee_SelectionMenu.WBP_Trainee_SelectionMenu_C");
	const TCHAR* SoldierRadioWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_SoldierRadioHand.WBP_SoldierRadioHand_C");
	const TCHAR* TraineeRoleWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trainee_Role.WBP_Trainee_Role_C");
	const TCHAR* TrackersWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Trackers.WBP_Trackers_C");
	const TCHAR* DangerDirectionWidget = TEXT("/Game/RS/Training/Blueprints/Safety/WBP_DangerDirection.WBP_DangerDirection_C");
	const TCHAR* AmmunitionWidget = TEXT("/Game/RS/Training/Blueprints/Widgets/WBP_Ammunition.WBP_Ammunition_C");
	const TCHAR* TrineeResultWidget = TEXT("/Game/RS/Control/Widgets/WBP_Trainee_Result.WBP_Trainee_Result_C");
}

namespace RtSctPath
{
	const TCHAR* RtAnimationPath = TEXT("/Game/CommonModels/SkeletalModels/Animations/Character");
	const TCHAR* RtAIControllerPath = TEXT("/Game/RS/Training/Blueprints/AI/BP_RtAIController.BP_RtAIController_C");
	const TCHAR* RtTraineeLocActor = TEXT("/Game/RS/Training/Blueprints/TraineeLocActor.TraineeLocActor_C");
}

namespace RtSctDefaultTexts
{
	const TCHAR* RtGoStartingPoint = TEXT("GoStartingPoint");
	const TCHAR* RtCheckTracker = TEXT("CheckTracker");
	const TCHAR* RtCalibrationFailed = TEXT("CalibrationFailed");
	const TCHAR* RtCalibrationComplete = TEXT("CalibrationComplete");
	const TCHAR* RtCalibrationPose = TEXT("CalibrationPose");
	const TCHAR* RtCalibrationPist = TEXT("CalibrationPist");
	const TCHAR* RtCalibrationPrepare = TEXT("CalibrationPrepare");
}