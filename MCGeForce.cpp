// MCGeForce.cpp : Defines the entry point for the application.
//

#include "MCGeForce.h"
#include <functional>
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

class MCGeForce {
public:
	const char* id;

	void init(const char* id);
	void addGroup(const char* id, const char *name);
	void setVideoHighlight(const char* highlightId, int start, int end);
	void showHighlightsEditor();
private:
	void requestPermission(void);

	int lastHighlightIndex = 0;
	NVGSDK_HANDLE* handle;
};

void asyncCallback(NVGSDK_RetCode ret, void* v) {
	string code = NVGSDK_RetCodeToString(ret);

	if (!NVGSDK_SUCCEEDED(ret)) {
		cout << "[MCGeForce] Error: " + code << endl;
	}
}

void MCGeForce::init(const char* id)
{	
	MCGeForce::id = id;
	NVGSDK_Scope scopes[] = { NVGSDK_SCOPE_HIGHLIGHTS, NVGSDK_SCOPE_HIGHLIGHTS_VIDEO, NVGSDK_SCOPE_HIGHLIGHTS_SCREENSHOT };
	NVGSDK_ScopePermission scopePermissions[3];

	NVGSDK_RetCode returnCode;

	NVGSDK_CreateInputParams createParams = { 0 };
	createParams.appName = "Minecraft";
	createParams.scopeTable = &scopes[0];
	createParams.scopeTableSize = 3;
	createParams.pollForCallbacks = false;
	
	NVGSDK_CreateResponse response;
	response.scopePermissionTable = &scopePermissions[0];
	response.scopePermissionTableSize = 3;

	returnCode = NVGSDK_Create(&handle, &createParams, &response);

	if (NVGSDK_SUCCEEDED(returnCode)) {
		cout << "[MCGeForce] Linked with GeForce Experience!" << endl;

		string version = response.gfeVersionStr;
		cout << "[MCGeForce] GeForce Experience Version: " + version << endl;

		requestPermission();
		addGroup(id, "MCGeForce");

		NVGSDK_Highlight highlights[4] = { 0 };

		NVGSDK_LocalizedPair highlight1Translation[1] = { { "en-US", "Kill" } };
		highlights[0].id = "mcg_kill";
		highlights[0].userInterest = true;
		highlights[0].highlightTags = NVGSDK_HIGHLIGHT_TYPE_MILESTONE;
		highlights[0].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_GOOD;
		highlights[0].nameTable = &highlight1Translation[0];
		highlights[0].nameTableSize = 1;

		NVGSDK_LocalizedPair highlight2Translation[1] = { { "en-US", "Game" } };
		highlights[1].id = "mcg_game";
		highlights[1].userInterest = true;
		highlights[1].highlightTags = NVGSDK_HIGHLIGHT_TYPE_STATE_CHANGE;
		highlights[1].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_NEUTRAL;
		highlights[1].nameTable = &highlight2Translation[0];
		highlights[1].nameTableSize = 1;

		NVGSDK_LocalizedPair highlight3Translation[1] = { { "en-US", "Win" } };
		highlights[2].id = "mcg_win";
		highlights[2].userInterest = true;
		highlights[2].highlightTags = NVGSDK_HIGHLIGHT_TYPE_ACHIEVEMENT;
		highlights[2].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_VERY_GOOD;
		highlights[2].nameTable = &highlight3Translation[0];
		highlights[2].nameTableSize = 1;

		NVGSDK_LocalizedPair highlight4Translation[1] = { { "en-US", "Death" } };
		highlights[3].id = "mcg_death";
		highlights[3].userInterest = true;
		highlights[3].highlightTags = NVGSDK_HIGHLIGHT_TYPE_MILESTONE;
		highlights[3].significance = NVGSDK_HIGHLIGHT_SIGNIFICANCE_BAD;
		highlights[3].nameTable = &highlight4Translation[0];
		highlights[3].nameTableSize = 1;

		NVGSDK_HighlightConfigParams params = { 0 };
		params.highlightDefinitionTable = &highlights[0];
		params.highlightTableSize = 4;
		params.defaultLocale = "en-US";
		NVGSDK_Highlights_ConfigureAsync(handle, &params, NULL, NULL);
	}
}

void MCGeForce::addGroup(const char* id, const char* name)
{
	NVGSDK_LocalizedPair group1Translation[1] = { { "en-US", name } };
	NVGSDK_HighlightOpenGroupParams ogParams = { 0 };
	ogParams.groupId = id;
	ogParams.groupDescriptionTable = group1Translation;
	ogParams.groupDescriptionTableSize = 1;
	NVGSDK_Highlights_OpenGroupAsync(handle, &ogParams, NULL, NULL);
}

void MCGeForce::setVideoHighlight(const char* highlightId, int start, int end)
{
	cout << id << endl;
	NVGSDK_VideoHighlightParams vhParams = { 0 };
	vhParams.highlightId = highlightId;
	vhParams.groupId = id;
	vhParams.startDelta = start;
	vhParams.endDelta = end;
	NVGSDK_Highlights_SetVideoHighlightAsync(handle, &vhParams, NULL, NULL);
}

void MCGeForce::showHighlightsEditor()
{
	NVGSDK_GroupView groupView = { 0 };
	groupView.groupId = id;

	NVGSDK_SummaryParams summaryParams = { 0 };
	summaryParams.groupSummaryTable = { &groupView };
	summaryParams.groupSummaryTableSize = 1;
	NVGSDK_Highlights_OpenSummaryAsync(handle, &summaryParams, NULL, NULL);
}

void MCGeForce::requestPermission(void)
{
	NVGSDK_Scope scopes[] = { NVGSDK_SCOPE_HIGHLIGHTS, NVGSDK_SCOPE_HIGHLIGHTS_VIDEO, NVGSDK_SCOPE_HIGHLIGHTS_SCREENSHOT };
	NVGSDK_ScopePermission scopePermissions[3];

	NVGSDK_RequestPermissionsParams params;
	params.scopeTable = &scopes[0];
	params.scopeTableSize = 3;

	NVGSDK_RequestPermissionsAsync(handle, &params, NULL, NULL);
}

static MCGeForce instance;

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_init
(JNIEnv* env, jobject obj, jstring arg1) {
	const char* arg1native = env->GetStringUTFChars(arg1, 0);

	instance.init(arg1native);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_setVideoHighlight
(JNIEnv* env, jobject obj, jstring arg1, jint arg2, jint arg3) {
	const char* arg1native = env->GetStringUTFChars(arg1, 0);

	instance.setVideoHighlight(arg1native, arg2, arg3);
}

JNIEXPORT void JNICALL Java_dev_decobr_mcgeforce_bindings_MCGeForceHelper_showHighlightsEditor
(JNIEnv* env, jobject obj) {
	instance.showHighlightsEditor();
}
