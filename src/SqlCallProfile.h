#ifndef SQLCALLPROFILE_H
#define SQLCALLPROFILE_H

#include "SBCCallProfile.h"
#include <pqxx/result>
#include <string>

#include "resources/Resource.h"
#include "db/DbTypes.h"

#define REFRESH_METHOD_INVITE					1
#define REFRESH_METHOD_UPDATE					2
#define REFRESH_METHOD_UPDATE_FALLBACK_INVITE	3

#define FILTER_TYPE_TRANSPARENT					0
#define FILTER_TYPE_BLACKLIST					1
#define FILTER_TYPE_WHITELIST					2

using std::string;

struct SqlCallProfile
	: public SBCCallProfile
#ifdef OBJECTS_COUNTER
  , ObjCounter(SqlCallProfile)
#endif
{
	int time_limit;
	int disconnect_code_id;
	int session_refresh_method_id;
	int aleg_session_refresh_method_id;
	int aleg_override_id,bleg_override_id;
	int dump_level_id;

	/** whether or not we should parse trusted headers from this gateway */
	bool trusted_hdrs_gw;

	AmArg dyn_fields;

	string resources;
	ResourceList rl;

	SqlCallProfile();
	~SqlCallProfile();

	static bool skip(const pqxx::row &t);
	bool readFromTuple(const pqxx::row &t,const DynFieldsT &df);
	bool readFilter(const pqxx::row &t, const char* cfg_key_filter,
			vector<FilterEntry>& filter_list, bool keep_transparent_entry,
			int failover_type_id = FILTER_TYPE_TRANSPARENT);
	bool readFilterSet(const pqxx::row &t, const char* cfg_key_filter,
			vector<FilterEntry>& filter_list);
	bool readCodecPrefs(const pqxx::row &t);
	bool readDynFields(const pqxx::row &t,const DynFieldsT &df);
	bool column_exist(const pqxx::row &t,string column_name);
	bool eval_media_encryption();
	bool eval_resources();
	bool eval_radius();
	bool eval_transport_ids();
	bool eval_protocol_priority();
	bool eval();

	void infoPrint(const DynFieldsT &df);
	void info(AmArg &s);
	SqlCallProfile *copy();
};

#endif // SQLCALLPROFILE_H
