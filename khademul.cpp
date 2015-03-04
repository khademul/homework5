/* Homework 5
   CIVL 8903
   Khademul Haque
   URL:https://github.com/khademul/homework5
*/

#include <iostream>
// fixes
#include <cstdio> // for printf
#include <cstring> // for atoi
#include <cstdlib> // for exit
#define EXIT_FAILURE 1

using namespace std;

FILE *fpLog = NULL;

struct StationInfo {
	char networkCode[3];
	char stationCode[6];
	enum {LongPeriod='L', ShortPeriod='B', Broadband='H'} typeOfBand;
	enum {HighGain='H', LowGain='L', Accelerometer='N'} typeOfInstrument;
	char orientation; // N, E, Z, 1, 2, 3
};

void printLog(char *strLog) {
	printf("%s\n", strLog);
	fprintf(fpLog, "%s\r\n", strLog);
}

int isIntegerString(char *str) {
	for(int i=0; i<strlen(str); i++) {
		if((str[i]>='0' && str[i]<='9') || str[i]=='-' || str[i]=='+') {

		} else {
			return 0;
		}
	}
	return 1;
}
int atoi_h(char *str) {
	if(!isIntegerString(str)) {
		printLog("Error: Integer Expected");
		exit(EXIT_FAILURE);
	}
	return atoi(str);
}

int isFloatString(char *str) {
	for(int i=0; i<strlen(str); i++) {
		if((str[i]>='0' && str[i]<='9') || str[i]=='.' || str[i]=='-' || str[i]=='+') {

		} else {
			return 0;
		}
	}
	return 1;
}

float atof_h(char *str) {
	if(!isFloatString(str)) {
		printLog("Error: Real Number Expected");
		printLog(str);
		exit(EXIT_FAILURE);
	}
	return atof(str);
}

int isEqual(char *s1, char *s2) {
	return strcmp(s1,s2)==0;
}

int isEqualCI(char *s1, char *s2) {
	if(strlen(s1)!=strlen(s2)) {
		return 0;
	}
	for(int i=0; i<strlen(s1); i++) {
		if(tolower(s1[i])!=tolower(s2[i])) {
			return 0;
		}
	}
	return 1;
}

void trimLastNewLines(char *str) {
	for(int i=strlen(str)-1; i>=0; i++) {
		if(str[i]=='\r' || str[i]=='\n') {
			str[i] = 0;
		} else {
			break;
		}
	}
}

int main() {
	char *line = NULL;
	char strTmp[256];
	char inputFilePath[1024];
	size_t len = 0;
	size_t read;

	printf("Input File: ");
	cin>>inputFilePath;


	fpLog = fopen("earthquake.log", "w");
	if(fpLog==NULL) {
		printf("Unable to open earthquake.log. Check permission\n");
		exit(EXIT_FAILURE);
	}


	sprintf(strTmp, "Opening file:%s",inputFilePath);
	printLog(strTmp);

	FILE *fp = fopen(inputFilePath, "r");
	if (fp == NULL) {
		printLog("Input file not exist");
		exit(EXIT_FAILURE);
	}

	printLog("Processing input...");
	char *eventId = NULL;
	if(getline(&eventId, &len, fp)==-1) {
		printLog("Error in Header File: No Event ID");
		return 0;
	}
	trimLastNewLines(eventId);
	if(getline(&line, &len, fp)==-1) {
		printLog("Error in Header File: Date Time Row Missing");
		return 0;
	}

	char *dateStr = strtok(line, " \r\n");
	if (dateStr == NULL) {
		printLog("Error in Header File: Date Error");
		return 0;
	}
	char *timeStr = strtok(NULL, " \r\n");
	if (timeStr == NULL) {
		printLog("Error in Header File: Time Error");
		return 0;
	}
	char *timezoneStr = strtok(NULL, " \r\n");
	char timezone[4];
	if (timezoneStr == NULL) {
		printLog("Error in Header File: Timezone Error");
		return 0;
	}
	if(strlen(timezoneStr)!=3) {
		printLog("Error in Header File: Timezone Format Error");
		return 0;
	}
	strcpy(timezone,timezoneStr);
	int year,month,day,hour,minute,second,milisecond;
	if ((dateStr[2]=='/' && dateStr[5]=='/') || (dateStr[2]=='-' && dateStr[5]=='-')) {
		month = atoi_h(strtok(dateStr,"/-"));
		day = atoi_h(strtok(NULL,"/-"));
		year = atoi_h(strtok(NULL,"/-"));
	} else {
		printLog("Error in Header File: Date format error");
		return 0;
	}
	if (timeStr[2]==':' && timeStr[5]==':' && timeStr[8]=='.') {
		hour = atoi_h(strtok(timeStr,":."));
		minute = atoi_h(strtok(NULL,":."));
		second = atoi_h(strtok(NULL,":."));
		milisecond = atoi_h(strtok(NULL,":."));
	} else {
		printLog("Error in Header File: Time format error");
		return 0;
	}
	char *earthquakeName = NULL;
	if(getline(&earthquakeName, &len, fp)==-1) {
		printLog("Error in Header File: Earthquake Name Missing");
		return 0;
	}
	trimLastNewLines(earthquakeName);

	if(getline(&line, &len, fp)==-1) {
		printLog("Error in Header File: Latitude Longitude Line Missing");
		return 0;
	}
	double longitude = atof_h(strtok(line," \r\n"));
	double latitude = atof_h(strtok(NULL," \r\n"));
	double depth = atof_h(strtok(NULL," \r\n"));

	char *magnitudeType = strtok(NULL," \r\n");
	double magnitudeSize = atof_h(strtok(NULL," \r\n"));


	if(isEqualCI(magnitudeType,"ml") || isEqualCI(magnitudeType,"ms") || isEqualCI(magnitudeType,"mb") || isEqualCI(magnitudeType,"mw")) {

	} else {
		printLog("Error in Header File: magnitude type error");
		return 0;
	}
	if(magnitudeSize<0) {
		printLog("Error in Header File: magnitude size should be positive number");
		return 0;
	}
	if(month<1 || month>12) {
		printLog("Invalid Month");
		return 0;
	}
	printLog("Header read correctly!");

	FILE *fpOut = fopen("earthquake.out", "w");
	if(fpOut==NULL) {
		printLog("Unable to open earthquake.out. Check permission");
		exit(EXIT_FAILURE);
	}

	const char* months[12] =
	{
	    "January","February","March","April","May","June",
	    "July","August","September","October","November","December"
	};
	fprintf(fpOut, "# %.2d %s %d %.2d:%.2d:%.2d.%.3d %s %s %.1f %s [%s] (%.2f %.2f %.1f)\n",
			day,months[month-1],year,hour,minute,second,milisecond,timezone,magnitudeType,
			magnitudeSize,earthquakeName,eventId,longitude,latitude,depth);

	StationInfo stationInfos[300];

	int entryNumber=1;
	int invalidCount=0;
	int validEntryCount=0;
	int totalSignalNames=0;
	while(getline(&line, &len, fp)!=-1) {
		int isValidRow=1;
		trimLastNewLines(line);
		//printf("%s\n", line);
		StationInfo stationInfo;
		char *networkCode = strtok(line," ");
		if(isEqual(networkCode,"CE") || isEqual(networkCode,"CI") || isEqual(networkCode,"FA")
				|| isEqual(networkCode,"NP") || isEqual(networkCode,"WR")) {

		} else {
			sprintf(strTmp, "Entry #%3d ignored. Invalid network.",entryNumber);
			printLog(strTmp);
			isValidRow=0;
			//invalidCount++;
			//continue;
		}
		strcpy(stationInfo.networkCode,networkCode);


		char *stationCode = strtok(NULL," ");
		int isValid=1;
		if (strlen(stationCode)==3) {
			for(int i=0;i<3;i++) {
				if(stationCode[i]<'A' || stationCode[i]>'Z') {
					isValid=0;
					break;
				}
			}
		} else if(strlen(stationCode)==5) {
			for(int i=0;i<5;i++) {
				if(stationCode[i]<'0' || stationCode[i]>'9') {
					isValid=0;
					break;
				}
			}
		} else {
			isValid=0;
		}
		if(isValid==0) {
			sprintf(strTmp, "Entry #%3d ignored. Invalid station name.",entryNumber);
			printLog(strTmp);
			isValidRow=0;
			//invalidCount++;
			//continue;
		}
		strcpy(stationInfo.stationCode,stationCode);

		char *typeOfBand = strtok(NULL," ");
		if(isEqualCI(typeOfBand,"Long-period")) {
			stationInfo.typeOfBand = StationInfo::LongPeriod;
		} else if(isEqualCI(typeOfBand,"Short-period")) {
			stationInfo.typeOfBand = StationInfo::ShortPeriod;
		} else if(isEqualCI(typeOfBand,"Broadband")) {
			stationInfo.typeOfBand = StationInfo::Broadband;
		} else {
			sprintf(strTmp, "Entry #%3d ignored. Invalid band type.",entryNumber);
			printLog(strTmp);
			//invalidCount++;
			//continue;
			isValidRow=0;
		}
		char *typeOfInstrument = strtok(NULL," ");
		if(isEqualCI(typeOfInstrument,"High-Gain")) {
			stationInfo.typeOfInstrument = StationInfo::HighGain;
		} else if(isEqualCI(typeOfInstrument,"Low-Gain")) {
			stationInfo.typeOfInstrument = StationInfo::LowGain;
		} else if(isEqualCI(typeOfInstrument,"Accelerometer")) {
			stationInfo.typeOfInstrument = StationInfo::Accelerometer;
		} else {
			sprintf(strTmp, "Entry #%3d ignored. Invalid type of instrument.",entryNumber);
			printLog(strTmp);
			//invalidCount++;
			//continue;
			isValidRow=0;
		}
		char *orientation = strtok(NULL," ");
		isValid=1;
		if(strlen(orientation)>3) {
			isValid=0;
		} else {
			if(orientation[0]>='0' && orientation[0]<='9') {
				for(int i=0;i<strlen(orientation);i++) {
					if(orientation[i]<'0' || orientation[i]>'9') {
						isValid=0;
					}
				}
			} else {
				for(int i=0;i<strlen(orientation);i++) {
					if((orientation[i]>='A' && orientation[i]<='Z') || (orientation[i]>='a' && orientation[i]<='z')) {

					} else {
						isValid=0;
					}
				}
			}
		}
		if(!isValid) {
			sprintf(strTmp, "Entry #%3d ignored. Invalid orientation.",entryNumber);
			printLog(strTmp);
			//invalidCount++;
			//continue;
			isValidRow=0;
		} else {

		}
		if(isValidRow==0){
			invalidCount++;
		} else {
			for(int i=0;i<strlen(orientation);i++) {
				strcpy(stationInfos[totalSignalNames].networkCode, stationInfo.networkCode);
				strcpy(stationInfos[totalSignalNames].stationCode, stationInfo.stationCode);
				stationInfos[totalSignalNames].orientation = orientation[i];
				stationInfos[totalSignalNames].typeOfBand = stationInfo.typeOfBand;
				stationInfos[totalSignalNames].typeOfInstrument = stationInfo.typeOfInstrument;
				totalSignalNames++;
			}
			validEntryCount++;
		}
		entryNumber++;
	}

	sprintf(strTmp,"Total invalid entries ignored: %d",invalidCount);
	printLog(strTmp);
	sprintf(strTmp,"Total valid entries read: %d",validEntryCount);
	printLog(strTmp);
	sprintf(strTmp,"Total signal names produced: %d",totalSignalNames);
	printLog(strTmp);

	fprintf(fpOut,"%d\n",totalSignalNames);
	for(int i=0;i<totalSignalNames;i++) {
		fprintf(fpOut,"%s.%s.%c%c%c\n",stationInfos[i].networkCode,stationInfos[i].stationCode
				,stationInfos[i].typeOfBand,stationInfos[i].typeOfInstrument,
				stationInfos[i].orientation);
	}

	fclose(fp);
	fclose(fpLog);
	fclose(fpOut);
	if(line) {
		free(line);
	}
	return 0;
}
