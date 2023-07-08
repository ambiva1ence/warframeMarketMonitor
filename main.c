#define _CRT_SECURE_NO_WARNINGS 1
#define BUFLEN 1024
#define MAXLINE 100000

#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include <Python.h>
#include <windows.h>

struct tagLists {
	int idx;
	char tagName[400];
	char* engName;
	struct tagLists* next;
};

typedef struct tagLists taglist;
typedef struct tagLists* taglist_ptr;

const char FILENAME[2048] = "configOutput.json";
const char GETNAME[2048] = "restoreOutputAuctions.json";
const char DAYCOUNT_DATA[2048] = "maxDayCount";
const int rangeAmount = 5;
const int __DEBUG = 1;

char weapon[100];
int tagsOpp = 0;
taglist_ptr banWeapon;
taglist_ptr normalList;
int printTags = 0;
int moneyMin = -1;
int moneyMax = -1;
int dayCount = 2;

taglist_ptr initNewTag() {
	taglist_ptr newTag = (taglist_ptr)(malloc(sizeof(taglist)));
	newTag->idx = 0;
	newTag->next = NULL;
	return newTag;
};

char* getEngChar(taglist_ptr insertingList, int index) {
	int found = 0;
	while (insertingList != NULL) {
		if (insertingList->idx == index) {
			return insertingList->engName;
		}
		insertingList = insertingList->next;
	}

	return "found Nothing";
}


char* getChar(taglist_ptr insertingList, int index) {
	int found = 0;
	while (insertingList != NULL) {
		if (insertingList->idx == index) {
			return insertingList->tagName;
		}
		insertingList = insertingList->next;
	}

	return "found Nothing";
}

void endInsert(taglist_ptr insertingList, char* tag, char* eng) {
	taglist_ptr newTag = initNewTag();
	taglist_ptr headTag = insertingList;
	int countNum = 1;
	while (insertingList->next != NULL) {
		countNum++;
		insertingList = insertingList->next;
	}

	newTag->idx = countNum;
	strcpy(newTag->tagName, tag);
	newTag->engName = eng;
	insertingList->next = newTag;
	headTag->idx = countNum;
}

taglist_ptr getList() {
	taglist_ptr newTag = initNewTag();
	endInsert(newTag, "������",			"critical_chance");
	endInsert(newTag, "�����˺�",			"critical_damage");
	endInsert(newTag, "�佹",			"zoom");
	endInsert(newTag, "�����˺�",			"cold_damage");
	endInsert(newTag, "����˺�",			"impact_damage");
	endInsert(newTag, "��������",			"status_chance");
	endInsert(newTag, "����ʱ��",			"status_duration");
	endInsert(newTag, "�����˺�",			"puncture_damage");
	endInsert(newTag, "��͸",			"punch_through");
	endInsert(newTag, "��ϻ����",			"magazine_capacity");
	endInsert(newTag, "��ҩ����",			"ammo_maximum");
	endInsert(newTag, "����˺�",			"electric_damage");
	endInsert(newTag, "�����˺�",			"toxin_damage");

	endInsert(newTag, "��Corpus�˺�",	"damage_vs_corpus");
	endInsert(newTag, "��Grineer�˺�",	"damage_vs_grineer");
	endInsert(newTag, "��Infested�˺�",  "damage_vs_infested");
	endInsert(newTag, "�������",			"multishot");
	endInsert(newTag, "������",			"recoil");
	endInsert(newTag, "�����˺�",			"heat_damage");
	endInsert(newTag, "�����˺�",			"base_damage_/_melee_damage");
	endInsert(newTag, "�и��˺�",			"slash_damage");

	endInsert(newTag, "����/�����ٶ�",	"fire_rate_/_attack_speed");
	endInsert(newTag, "Ͷ��������ٶ�",	"projectile_speed");
	endInsert(newTag, "װ���ٶ�",			"reload_speed");

	endInsert(newTag, "��ʼ������",		"channeling_damage");
	endInsert(newTag, "�����˺�",			"finisher_damage");
	endInsert(newTag, "������������ȡ",	"chance_to_gain_extra_combo_count");
	endInsert(newTag, "������Χ",			"range");
	endInsert(newTag, "���й���������",	"critical_chance_on_slide_attack");
	endInsert(newTag, "��������ʱ��",		"combo_duration");
	endInsert(newTag, "�ػ�Ч��",			"channeling_efficiency");
	endInsert(newTag, "�и������", "has");
	endInsert(newTag, "�޸������", "none");

	return newTag;
}



int endInsertValue(taglist_ptr insertingList, int value) {
	if (value >= 31) {
		return 0;
	}

	taglist_ptr newTag = initNewTag();
	taglist_ptr headTag = insertingList;
	int countNum = 1;
	int shouldInvert = 1;
	while (insertingList->next != NULL) {
		countNum++;
		insertingList = insertingList->next;
		if (value == insertingList->idx || value == tagsOpp) {
			shouldInvert = 0;
		}
	}
	
	if (shouldInvert == 1) {
		newTag->idx = value;
		strcpy(newTag->tagName, getChar(getList()->next, value));
		newTag->engName = getEngChar(getList()->next, value);
		insertingList->next = newTag;
		headTag->idx = countNum; 
	}

	return shouldInvert;
}




int endInsertChar(taglist_ptr insertingList) {
	char wpn[2024] = {'\0'};
	scanf("%s", wpn);
	taglist_ptr newTag = initNewTag();
	taglist_ptr topTag = insertingList;
	taglist_ptr headTag = insertingList;
	int countNum = 1;
	int shouldInvert = 1;
	while (headTag->next != NULL) {
		countNum++;
		headTag = headTag->next;
	}

	if (shouldInvert == 1) {
		newTag->idx = countNum;
		strcpy(newTag->tagName, wpn);
		newTag->engName = "none";
		headTag->next = newTag;
		topTag->idx = countNum;
	}

	return shouldInvert;
}

void printList(taglist_ptr insertingList) {
	insertingList = insertingList->next;
	int countNum = 1;
	while (insertingList != NULL) {
		countNum++;
		printf("  %2d:%-15s\t", insertingList->idx, insertingList->tagName);
		insertingList = insertingList->next;
		if ((countNum - 1) % 2 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

void printListTag(taglist_ptr insertingList) {
	insertingList = insertingList->next;
	while (insertingList != NULL) {
		char str2[20];

		strcpy(str2, insertingList->tagName);
		printf(" %d:%s; ", insertingList->idx, str2);
		insertingList = insertingList->next;
	}
	printf("\n");
}

void funcInit() {
	if (__DEBUG == 1)
		system("cls");

	printf("%s", "\n �ܴ���-���������£�\n");
	taglist_ptr mainList = getList();
	printList(mainList);
	printf(" ָ�:\n");
	printf("  %-22s:%-15s|%-22s:%-15s|%-22s:%-15s\n", "[/pos_add +����]", "�����������", "[/opp_add +����]", "���Ӹ������", "[/wpn_add +����]", "�޸���������");
	printf("  %-22s:%-15s|%-22s:%-15s|%-22s:%-15s\n", "[/set_money_min +����]", "�޸���С��Ǯ", "[/set_money_min +����]", "�޸�����Ǯ", "[/monitor]", "��ʼ���");
	printf("  %-22s:%-15s|%-22s:%-15s|%-22s:%-15s\n", "[/pos_reset]", "�����������", "[/opp_reset]", "���ø������", "[/wpn_reset]", "������������");
	printf("  %-22s:%-15s|%-22s:%-15s\n", "[/ban_wpn +����]", "��ֹ����", "[/unban_wpn +����]", "�������");
	printf("  %-22s:%-15s|%-22s:%-15s|%-22s:%-15s\n", "[/reset]", "��������", "[/exit]", "�˳�����", "[/search]", "��������");
	printf("  [�������ռ�ȫ����]\n");
}

void delete(taglist_ptr list, char* value) {
	taglist_ptr preptr = list;
	list = list->next;
	taglist_ptr currentptr = list;
	while (currentptr != NULL) {
		if (strcmp(currentptr->tagName, value) == 0) {
			preptr->next = currentptr->next;
			free(currentptr);
			break;
		}

		preptr = preptr->next;
		currentptr = currentptr->next;
	}
}

void sprintTags() {
	if (printTags >= 1) {
		printf(" [�Ѽ���]�������:");
		printListTag(normalList);
	}
	if (tagsOpp != 0) {
		printf(" [�Ѽ���]�������: %d:%s;\n", tagsOpp, getChar(getList()->next, tagsOpp));
	}
	if (strlen(weapon) > 0) {
		printf(" [�Ѽ���]��������: %s;\n", weapon);
	}
	if (moneyMax >= 0 || moneyMin >= 0) {
		printf(" [�Ѽ���]�۸�����: ��С:%d - ���:%d;\n", moneyMin, moneyMax);
	}
}

int scanNormalList(value) {
	taglist_ptr head = normalList;
	head = head->next;
	int state = 0;
	while (head != NULL) {
		if (head->idx == value) {
			state = 1;
		}
		head = head->next;
	}

	return state;
}

void createJsonInfo() {
	taglist_ptr head = normalList->next;
	taglist_ptr bHead = banWeapon->next;
	cJSON* root = NULL;
	char* out = NULL;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "type", "riven");
	cJSON_AddStringToObject(root, "polarity", "any");
	cJSON_AddStringToObject(root, "sort_by", "price_asc");
	cJSON* positive_stats = cJSON_CreateArray();
	cJSON* banWpns = cJSON_CreateArray();
	while (head) {
		cJSON_AddItemToArray(positive_stats, cJSON_CreateString(head->engName));
		head = head->next;
	}

	while (bHead) {
		cJSON_AddItemToArray(banWpns, cJSON_CreateString(bHead->tagName));
		bHead = bHead->next;
	}

	cJSON_AddItemToObject(root, "ban_wpns", banWpns);
	cJSON_AddItemToObject(root, "positive_stats", positive_stats);
	cJSON_AddStringToObject(root, "negative_stats", ((tagsOpp == 0) ? "NULL" : getEngChar(getList()->next, tagsOpp)));
	cJSON_AddStringToObject(root, "weapon_url_name", ((weapon[0] == '\0') ? "NULL" : weapon));
	cJSON_AddNumberToObject(root, "range_money_min", moneyMin);
	cJSON_AddNumberToObject(root, "range_money_max", moneyMax);
	cJSON_AddNumberToObject(root, "range_amount", rangeAmount);
	out = cJSON_Print(root);
	
	FILE* file = NULL;
	file = fopen(FILENAME, "w");
	if (file == NULL) {
		printf("Open file fail��\n");
		cJSON_Delete(root);
		return;
	}

	char* cjValue = cJSON_Print(root);
	int ret = fputs(cjValue, file);
	if (ret == EOF) {
		printf("д���ļ�ʧ�ܣ�\n");
	}

	fclose(file);
	free(cjValue);
}

char* citoa(int num, char* str, int radix)
{
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;
	int i = 0, j, k;

	if (radix == 10 && num < 0)
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else unum = (unsigned)num;

	do
	{
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;

	} while (unum);

	str[i] = '\0';

	if (str[0] == '-') k = 1;
	else k = 0;

	char temp;
	for (j = k; j <= (i - 1) / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}

	return str;

}

//void initBan_and_Counts() {
//	cJSON* cjson_test = NULL;
//	cJSON* cjson_banWpns = NULL;
//	int    wpns_array_size = 0, i = 0;
//	cJSON* cjson_banWpn = NULL;
//
//	FILE* file;
//	char buf[BUFLEN];
//	char* array[MAXLINE];
//	file = fopen(FILENAME, "r");
//	if (!file)return -1;
//	fgets(buf, BUFLEN, file);
//	fclose(file);
//
//	FILE* Dfile;
//	Dfile = fopen(DAYCOUNT_DATA, "rt");
//	char ch = fgetc(Dfile);
//	fclose(Dfile);
//
//	dayCount = (int)ch;
//
//	cjson_test = cJSON_Parse(buf);
//
//	cjson_banWpns = cJSON_GetObjectItem(cjson_test, "ban_wpns");
//	wpns_array_size = cJSON_GetArraySize(cjson_banWpns);
//	printf("%d", wpns_array_size);
//	for (i = 0; i < wpns_array_size; i++)
//	{
//		printf("123");
//		cjson_banWpn = cJSON_GetArrayItem(cjson_banWpns, i);
//		endInsert(banWeapon, cjson_banWpn->valuestring, "none");
//	}
//}

void getInput() {
	cJSON* cjson_test = NULL;
	cJSON* cjson_amount = NULL;
	cJSON* cjson_content = NULL;
	cJSON* cjson_owner = NULL;
	cJSON* cjson_weapon_name = NULL;
	cJSON* cjson_price = NULL;
	cJSON* cjson_opposite_name = NULL;
	cJSON* cjson_id = NULL;
	cJSON* cjson_name = NULL;
	char string[16] = { 0 };

	FILE* file;
	char buf[BUFLEN];
	int len = 0, i = 0;
	char* array[MAXLINE];

	file = fopen(GETNAME, "r");
	if (!file)return -1;

	fgets(buf, BUFLEN, file);

	fclose(file);
	cjson_test = cJSON_Parse(buf);
	while (cjson_test == NULL) {
		cjson_test = cJSON_Parse(buf);
	}

	cjson_amount = cJSON_GetObjectItem(cjson_test, "amount");

	for (int i = 0; i < rangeAmount; i++) {
		citoa(i, string, 10);
		cjson_content = cJSON_GetObjectItem(cjson_test, string);
		if (cjson_content != NULL) {
			cjson_owner = cJSON_GetObjectItem(cjson_content, "owner");
			cjson_weapon_name = cJSON_GetObjectItem(cjson_content, "weapon_name");
			cjson_opposite_name = cJSON_GetObjectItem(cjson_content, "opposite_name");
			cjson_price = cJSON_GetObjectItem(cjson_content, "price");
			cjson_name = cJSON_GetObjectItem(cjson_content, "name");
			cjson_id = cJSON_GetObjectItem(cjson_content, "id");
			printf("\n");
			printf("  %d:Mod��:%s - ������:%s - �۸�:%d - �������:%s\n", i + 1, cjson_name->valuestring, cjson_weapon_name->valuestring, cjson_price->valueint, cjson_opposite_name->valuestring);
			printf("   ��������: https://warframe.market/auction/%s\n", cjson_id->valuestring);
			printf("   �������: /w %s hi! i want to buy your [%s %s] for %dpl\n", cjson_owner->valuestring, cjson_weapon_name->valuestring, cjson_name->valuestring, cjson_price->valueint);
		}
	}
}

void resetFunc() {
	normalList = initNewTag();
	printTags = 0;
	tagsOpp = 0;
	weapon[0] = '\0';
	moneyMax = -1;
	moneyMax = -1;
}

void pythonCommanding() {
	char* ret = "NULL";

	Py_Initialize();

	PyObject* pModule;
	PyObject* pFunction;
	PyObject* pArgs;
	PyObject* pRetValue;

	pModule = PyImport_ImportModule("getHttpCommand");
	if (!pModule) {
		PyErr_Print();
		printf("import python failed1!!\n");
		return -1;
	}

	pFunction = PyObject_GetAttrString(pModule, "main_func");
	if (!pFunction) {
		printf("get python function failed!!!\n");
		return -1;
	}

	pArgs = PyTuple_New(0);
	pRetValue = PyObject_CallObject(pFunction, pArgs);

	Py_Finalize();
}

int main()
{
	banWeapon = initNewTag();
	normalList = initNewTag();
	SetConsoleTitle(L"���ô�ԭ���������ɼ���");

	int stateExit = 0;
	char command[20];

	while (stateExit != 1) {
		funcInit();
		sprintTags();
		printf("\n");
		printf("%s", " ������ָ��:");

		scanf("%s", (command));

		if (strcmp(command, "/pos_add") == 0) {
			if (printTags + 1 <= 3) {
				int getNum = 0;
				funcInit();
				sprintTags();
				printf("\n");
				printf("%s", " [NUMBER] ����������:");
				scanf("%d", (&getNum));

				int insertSuccess = endInsertValue(normalList, getNum);
				if (insertSuccess == 1)
					printTags++;
			}
		}
		if (strcmp(command, "/ban_wpn") == 0) {
			funcInit();
			sprintTags();
			printf("\n");
			printf("%s", " [NUMBER] ������������:");
			endInsertChar(banWeapon);
			printListTag(banWeapon);
		}
		if (strcmp(command, "/unban_wpn") == 0) {
			char wpn[2024];
			funcInit();
			sprintTags();
			printf("\n");
			printf("%s", " [NUMBER] ������������:");
			scanf("%s", wpn);
			delete(banWeapon, wpn);
		}
		else if (strcmp(command, "/wpn_add") == 0) {
			funcInit();
			sprintTags();
			printf("\n");
			printf("%s", " [NUMBER] ��������������:");
			scanf("%s", weapon);
		}
		else if (strcmp(command, "/opp_add") == 0) {
			funcInit();
			sprintTags();
			int getNum = 0;
			printf("\n");
			printf("%s", " [NUMBER] ����������:");
			scanf("%d", (&getNum));

			if (scanNormalList(getNum) == 0) {
				tagsOpp = getNum;
			}	
		}
		else if (strcmp(command, "/set_money_min") == 0) {
			funcInit();
			sprintTags();
			printf("\n");
			printf("%s", " [NUMBER] ��������С�۸�:");
			scanf("%d", (&moneyMin));
		}
		else if (strcmp(command, "/set_money_max") == 0) {
			funcInit();
			sprintTags();
			printf("\n");
			printf("%s", " [NUMBER] ���������۸�:");
			scanf("%d", (&moneyMax));
		}
		else if (strcmp(command, "/exit") == 0) {
			stateExit = 1;
		}
		else if (strcmp(command, "/pos_reset") == 0) {
			normalList = initNewTag();
			printTags = 0;
		}
		else if (strcmp(command, "/opp_reset") == 0) {
			tagsOpp = 0;
		}
		else if (strcmp(command, "/wpn_reset") == 0) {
			weapon[0] = '\0';
		}
		else if (strcmp(command, "/reset") == 0) {
			resetFunc();
		}
		else if (strcmp(command, "/search") == 0) {
			createJsonInfo();
			resetFunc();
			pythonCommanding();
			Sleep(3000);
			getInput();
			scanf("%s", command);
		}
		else if (strcmp(command, "/monitor") == 0) {
			createJsonInfo();
			resetFunc();
			while (1) {
				pythonCommanding();
				Sleep(1000);
				funcInit();
				sprintTags();
				printf("\n �����\n");
				getInput();
			}
		}
	}

	return 0;
}