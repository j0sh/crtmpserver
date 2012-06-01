/*
 *  Copyright (c) 2010,
 *  Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 *
 *  This file is part of crtmpserver.
 *  crtmpserver is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  crtmpserver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with crtmpserver.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAS_VOD_MANAGER
#include "vod/basevodmanager.h"
#include "mediaformats/flv/flvdocument.h"
#include "mediaformats/mp3/mp3document.h"
#include "mediaformats/mp4/mp4document.h"
#include "mediaformats/nsv/nsvdocument.h"

BaseVODManager::BaseVODManager(BaseClientApplication *pApplication,
		Variant &configuration) {
	_pApplication = pApplication;
	_configuration = configuration["storage"];
}

BaseVODManager::~BaseVODManager() {
}

bool BaseVODManager::Initialize() {
	if (_configuration == V_NULL)
		return true;
	if (_configuration != V_MAP) {
		FATAL("Invalid storage node");
		return false;
	}
	if ((!_configuration.HasKeyChain(V_MAP, false, 1, "origins"))
			|| (_configuration.GetValue("origins", false).MapSize() == 0)
			) {
		FATAL("Invalid origins node specified in storage");
		return false;
	}

	map<string, string> temp;
	string value;
	string realPath;

	FOR_MAP(_configuration.GetValue("origins", false), string, Variant, i) {
		Variant &item = MAP_VAL(i);
		if (item != V_STRING) {
			FATAL("Invalid origins node specified in storage: %s", STR(item.ToString()));
			continue;
		}
		value = (string) item;
		trim(value);
		if (value == "") {
			FATAL("Invalid origins node specified in storage");
			return false;
		}
		realPath = normalizePath(value, "");
		if (realPath == "") {
			WARN("Invalid origin path specified: `%s`", STR(value));
			continue;
		}
		if (realPath[realPath.size() - 1] != PATH_SEPARATOR) {
			realPath += PATH_SEPARATOR;
		}
		if (MAP_HAS1(temp, realPath)) {
			WARN("Duplicate origin value detected: value: `%s`; realPath: `%s`", STR(value), STR(realPath));
			continue;
		}
		temp[realPath] = realPath;
		ADD_VECTOR_END(_origins, realPath);
	}

	if ((_configuration.HasKeyChain(V_MAP, false, 1, "speedCaches"))
			&& (_configuration.GetValue("speedCaches", false).MapSize() != 0)
			) {

		FOR_MAP(_configuration.GetValue("speedCaches", false), string, Variant, i) {
			Variant &item = MAP_VAL(i);
			if (item != V_STRING) {
				WARN("Invalid speedCaches node specified in storage: %s", STR(item.ToString()));
				continue;
			}
			value = (string) item;
			trim(value);
			if (value == "") {
				FATAL("Invalid speedCaches node specified in storage");
				return false;
			}
			realPath = normalizePath(value, "");
			if (realPath == "") {
				WARN("Invalid speed cache path specified: `%s`", STR(value));
				continue;
			}
			if (realPath[realPath.size() - 1] != PATH_SEPARATOR) {
				realPath += PATH_SEPARATOR;
			}

			if (MAP_HAS1(temp, realPath)) {
				WARN("Duplicate speed cache value detected: value: `%s`; realPath: `%s`", STR(value), STR(realPath));
				continue;
			}
			temp[realPath] = realPath;
			ADD_VECTOR_END(_speedCache, realPath);
		}
	}

	if (!_configuration.HasKeyChain(V_STRING, false, 1, "metadataFolder")) {
		FATAL("Invalid metadata folder specified in storage");
		return false;
	}
	value = (string) _configuration.GetValue("metadataFolder", false);
	trim(value);
	if (value == "") {
		FATAL("Invalid metadata folder specified: `%s`", STR(value));
		return false;
	}
	realPath = normalizePath(value, "");
	if (realPath == "") {
		FATAL("Invalid metadata folder specified: `%s`", STR(value));
		return false;
	}
	if (realPath[realPath.size() - 1] != PATH_SEPARATOR) {
		realPath += PATH_SEPARATOR;
	}
	if (MAP_HAS1(temp, realPath)) {
		FATAL("Metadata folder is one of origins or caches: value: `%s`; realPath: `%s`", STR(value), STR(realPath));
		return false;
	}
	_metadataFolder = realPath;

	if (!_configuration.HasKeyChain(_V_NUMERIC, true, 2, META_MEDIA_SEEK_SETTINGS,
			META_MEDIA_GRANULARITY)) {
		FATAL("invalid seekSettings.granularity value");
		return false;
	}

	if (!_configuration.HasKeyChain(V_BOOL, true, 2, META_MEDIA_SEEK_SETTINGS,
			META_MEDIA_KEYFRAME_SEEK)) {
		FATAL("invalid seekSettings.keyframeSeek value");
		return false;
	}

	_seekSettings[META_MEDIA_GRANULARITY] = (uint32_t) ((double) _configuration[META_MEDIA_SEEK_SETTINGS][META_MEDIA_GRANULARITY]*1000.0);
	_seekSettings[META_MEDIA_KEYFRAME_SEEK] = _configuration[META_MEDIA_SEEK_SETTINGS][META_MEDIA_KEYFRAME_SEEK];
	_seekSettings.Compact();

	return true;
}

bool BaseVODManager::GetVODSourceByFileName(string type,
		string fileName, Variant &metaData) {
	for (uint32_t i = 0; i < _origins.size(); i++) {
		//FINEST("inspect origin `%s` for file `%s`", STR(_origins[i]), STR(fileName));
		string path = normalizePath(_origins[i], fileName);
		if (path == "") {
			if (fileExists(_origins[i] + fileName)) {
				WARN("Possible break in attempt: origin: `%s`; fileName: `%s`",
						STR(_origins[i]), STR(fileName));
			}
			continue;
		}
		return GetVODSource(type, _origins[i], path, metaData);
	}

	return false;
}

bool BaseVODManager::GetVODSourceByAdobeNotation(string streamName,
		Variant &metaData) {
	metaData[META_MEDIA_FILE_PATHS][META_MEDIA_ADOBE_STREAM_NAME] = streamName;
	trim(streamName);
	if (streamName == "") {
		FATAL("Invalid stream name");
		return false;
	}

	//2. Split the file name into formatName and name
	string type = "";
	string name = "";
	string::size_type pos = streamName.find(':');
	if (pos != string::npos) {
		type = streamName.substr(0, pos);
		name = streamName.substr(pos + 1);
	} else {
		type = "";
		name = streamName;
	}
	trim(type);
	type = lowerCase(type);
	trim(name);
	if (name == "") {
		FATAL("Invalid stream name: `%s`", STR(streamName));
		return false;
	}

	//3. compute the complete file name
	string fileName = "";
	if ((type == "")
			|| (type == MEDIA_TYPE_FLV)
			|| (type == MEDIA_TYPE_LIVE_OR_FLV)) {
		type = MEDIA_TYPE_FLV;
		fileName = name + "."MEDIA_TYPE_FLV;
	} else if (type == MEDIA_TYPE_MP4) {
		fileName = name;
	} else if (type == MEDIA_TYPE_MP3) {
		fileName = name + "."MEDIA_TYPE_MP3;
	} else {
		FATAL("Invalid type: `%s`", STR(type));
		return false;
	}
	//FINEST("type: `%s`; name: `%s`; fileName: `%s`", STR(type), STR(name), STR(fileName));

	//4. get the data
	return GetVODSourceByFileName(type, fileName, metaData);
}

bool BaseVODManager::GetAsyncVODSourceByFile(string originFilePath, Variant &parameters) {
	NYIR;
}

bool BaseVODManager::GetAsyncVODSourceByAdobeNotation(string streamName, Variant &parameters) {
	NYIR;
}

bool BaseVODManager::GetVODSource(string &type, string &originFolder,
		string &originFilePath, Variant &metaData) {
	//1. compute the md5 for the name
	string originFilePathMd5 = md5(originFilePath, true);

	//2. see if we have a cached file on speed caches
	string cacheFilePath = "";
	string cacheFolder = "";
	for (uint32_t i = 0; i < _speedCache.size(); i++) {
		//FINEST("inspect cache `%s` for file `%s`", STR(_speedCache[i]), STR(originFilePathMd5));
		cacheFilePath = normalizePath(_speedCache[i], originFilePathMd5);
		if (cacheFilePath != "") {
			cacheFolder = _speedCache[i];
			break;
		}
	}

	//3. compute seek and meta file paths
	string seekFilePath = normalizePath(_metadataFolder, originFilePathMd5 + "."MEDIA_TYPE_SEEK);
	string metaFilePath = normalizePath(_metadataFolder, originFilePathMd5 + "."MEDIA_TYPE_META);
	string infoFilePath = normalizePath(_metadataFolder, originFilePathMd5 + "."MEDIA_TYPE_INFO);

	//4. get the last modification dates
	double originDate = getFileModificationDate(originFilePath);
	double cacheDate = cacheFilePath == "" ? 0 : getFileModificationDate(cacheFilePath);
	double seekDate = seekFilePath == "" ? 0 : getFileModificationDate(seekFilePath);
	double metaDate = metaFilePath == "" ? 0 : getFileModificationDate(metaFilePath);
	double infoDate = infoFilePath == "" ? 0 : getFileModificationDate(infoFilePath);

	//	FINEST("------------BEFORE--------------------------");
	//	FINEST("_metadataFolder: `%s`", STR(_metadataFolder));
	//	FINEST("type: `%s`", STR(type));
	//	FINEST("originFilePathMd5: `%s`", STR(originFilePathMd5));
	//	FINEST("originFilePath: `%s`; %.2f", STR(originFilePath), originDate);
	//	FINEST("cacheFilePath: `%s`; %.2f", STR(cacheFilePath), cacheDate);
	//	FINEST("seekFilePath: `%s`; %.2f", STR(seekFilePath), seekDate);
	//	FINEST("metaFilePath: `%s`; %.2f", STR(metaFilePath), metaDate);
	//	FINEST("infoFilePath: `%s`; %.2f", STR(infoFilePath), infoDate);

	//5. Delete old files
	if ((originDate >= cacheDate) && (cacheDate != 0)) {
		FINEST("Delete outdated file %s", STR(cacheFilePath));
		if (!deleteFile(cacheFilePath)) {
			WARN("Unable to delete file %s", STR(cacheFilePath));
		}
		cacheFilePath = "";
		cacheFolder = "";
		cacheDate = 0;
	}
	if ((originDate >= seekDate) && (seekDate != 0)) {
		FINEST("Delete outdated file %s", STR(seekFilePath));
		if (!deleteFile(seekFilePath)) {
			WARN("Unable to delete file %s", STR(seekFilePath));
		}
		seekFilePath = "";
		seekDate = 0;
	}
	if ((originDate >= metaDate) && (metaDate != 0)) {
		FINEST("Delete outdated file %s", STR(metaFilePath));
		if (!deleteFile(metaFilePath)) {
			WARN("Unable to delete file %s", STR(metaFilePath));
		}
		metaFilePath = "";
		metaDate = 0;
	}
	if ((originDate >= infoDate) && (infoDate != 0)) {
		FINEST("Delete outdated file %s", STR(infoFilePath));
		if (!deleteFile(infoFilePath)) {
			WARN("Unable to delete file %s", STR(infoFilePath));
		}
		infoFilePath = "";
		infoDate = 0;
	}

	//6. If one of the meta or seek file is missing, reset the other one as well
	if ((seekDate == 0) || (metaDate == 0)) {
		seekFilePath = "";
		seekDate = 0;
		metaFilePath = "";
		metaDate = 0;
	}
	//	FINEST("------------AFTER--------------------------");
	//	FINEST("_metadataFolder: `%s`", STR(_metadataFolder));
	//	FINEST("type: `%s`", STR(type));
	//	FINEST("originFilePathMd5: `%s`", STR(originFilePathMd5));
	//	FINEST("originFilePath: `%s`; %.2f", STR(originFilePath), originDate);
	//	FINEST("cacheFilePath: `%s`; %.2f", STR(cacheFilePath), cacheDate);
	//	FINEST("seekFilePath: `%s`; %.2f", STR(seekFilePath), seekDate);
	//	FINEST("metaFilePath: `%s`; %.2f", STR(metaFilePath), metaDate);
	//	FINEST("infoFilePath: `%s`; %.2f", STR(infoFilePath), infoDate);

	//7. if seek file is missing, regenerate it. This will also regenerate the metadata
	if (seekDate == 0) {
		metaData[META_MEDIA_TYPE] = type;
		metaData[META_MEDIA_FILE_PATHS][META_MEDIA_ORIGIN] = originFilePath;
		metaData[META_MEDIA_FILE_PATHS][META_MEDIA_CACHE] = cacheFilePath;
		metaData[META_MEDIA_FILE_PATHS][META_MEDIA_SEEK] = _metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_SEEK;
		metaData[META_MEDIA_FILE_PATHS][META_MEDIA_META] = _metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META;
		metaData[META_MEDIA_FILE_PATHS][META_MEDIA_INFO] = _metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_INFO;
		metaData[META_MEDIA_FOLDERS][META_MEDIA_ORIGIN] = originFolder;
		metaData[META_MEDIA_FOLDERS][META_MEDIA_CACHE] = cacheFolder;
		metaData[META_MEDIA_FOLDERS][META_MEDIA_META] = _metadataFolder;
		metaData[META_MEDIA_SEEK_SETTINGS] = _seekSettings;

		if (!GenerateMetaAndSeekFiles(metaData)) {
			FATAL("Unable to generate meta and seek files");
			return false;
		}
	} else {
		metaData.Reset();
		if (!Variant::DeserializeFromXmlFile(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META, metaData)) {
			FATAL("Unable to deserialize metadata file %s", STR(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META));
			deleteFile(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META);
			return false;
		}
		if (metaData[META_MEDIA_SEEK_SETTINGS] != _seekSettings) {
			WARN("Seek settings changed. Regenerate them");
			if (!deleteFile(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META)) {
				FATAL("Unable to delete file %s",
						STR(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_META));
				return false;
			}
			if (!deleteFile(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_SEEK)) {
				FATAL("Unable to delete file %s",
						STR(_metadataFolder + originFilePathMd5 + "."MEDIA_TYPE_SEEK));
				return false;
			}
			return GetVODSource(type, originFolder, originFilePath, metaData);
		}
	}

	return true;
}

bool BaseVODManager::GenerateMetaAndSeekFiles(Variant &metaData) {
	//1. Create the document
	BaseMediaDocument *pDocument = NULL;
	if (false) {

	}
#ifdef HAS_MEDIA_FLV
	else if (metaData[META_MEDIA_TYPE] == MEDIA_TYPE_FLV) {
		pDocument = new FLVDocument(metaData);
	}
#endif /* HAS_MEDIA_FLV */
#ifdef HAS_MEDIA_MP3
	else if (metaData[META_MEDIA_TYPE] == MEDIA_TYPE_MP3) {
		pDocument = new MP3Document(metaData);
	}
#endif /* HAS_MEDIA_MP3 */
#ifdef HAS_MEDIA_MP4
	else if (metaData[META_MEDIA_TYPE] == MEDIA_TYPE_MP4) {
		pDocument = new MP4Document(metaData);
	}
#endif /* HAS_MEDIA_MP4 */
#ifdef HAS_MEDIA_NSV
	else if (metaData[META_MEDIA_TYPE] == MEDIA_TYPE_NSV) {
		pDocument = new NSVDocument(metaData);
	}
#endif /* HAS_MEDIA_NSV */
	else {
		FATAL("File type not supported yet. Partial metadata:\n%s",
				STR(metaData.ToString()));
		return false;
	}

	//2. Process the document
	INFO("Generate seek/meta files for `%s`", STR(metaData[META_MEDIA_FILE_PATHS][META_MEDIA_ORIGIN]));
	if (!pDocument->Process()) {
		FATAL("Unable to process document");
		delete pDocument;
		WARN("!!!!!!!!!!CONF_APPLICATION_RENAMEBADFILES not yet implemented!!!!!!!");
		//		if ((bool)metaData[CONF_APPLICATION_RENAMEBADFILES]) {
		//			moveFile(metaData[META_SERVER_FULL_PATH],
		//					(string) metaData[META_SERVER_FULL_PATH] + ".bad");
		//		} else {
		//			WARN("File %s will not be renamed",
		//					STR(metaData[META_SERVER_FULL_PATH]));
		//		}
		return false;
	}

	//3. cleanup
	delete pDocument;

	//4. Done
	return true;
}
#endif /* HAS_VOD_MANAGER */

