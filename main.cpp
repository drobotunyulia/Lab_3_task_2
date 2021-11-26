#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <json/json.h>
#include "logger.h"

enum json_status
{
    JSON_ERROR = -1,
    JSON_SUCCESS = 0
};

Json::Value load_json_file(std::string const& file_name)
{
  Json::Value root;
  Json::Reader reader;
  std::ifstream ifs(file_name, std::ifstream::binary);
  reader.parse(ifs, root, false);
  return root;
}

json_status get_json_object(std::string const& file_name, Json::Value &root)
{
    root = load_json_file(file_name);
    if (!root || root["object"] != "logger")
    {
        return JSON_ERROR;
    }
    if (!root["streams"] || !root["streams"].isArray())
    {
        return JSON_ERROR;
    }
    for (unsigned int i = 0; i < root["streams"].size(); i++)
    {
        if (!root["streams"][i]["name"] ||
                !root["streams"][i]["name"].isString() ||
                !root["streams"][i]["severity"] ||
                !root["streams"][i]["severity"].isInt())
        {
            return JSON_ERROR;
        }
        if (root["streams"][i].size() !=2)
        {
            return JSON_ERROR;
        }
        if (root["streams"][i]["severity"] < 1 || root["streams"][i]["severity"] > 6)
        {
            return JSON_ERROR;
        }
    }
    return JSON_SUCCESS;
}

int main()
{
    Json::Value root;
    if (get_json_object("c:/log_task_2/logger.cfg", root) != JSON_SUCCESS)
    {
        std::cout << "json configuration file error..." << std::endl;
        return JSON_ERROR;
    }
    logger_builder *builder = new logger_builder;
    logger *logger_1 = builder->add_stream("c:/log_task_2/c_log_cfg/log_file_1.log", INFORMATION).
                                add_stream("c:/log_task_2/c_log_cfg/log_file_2.log", INFORMATION).
                                add_stream("c:/log_task_2/c_log_cfg/log_file_3.log", TRACE).
                                add_stream("c:/log_task_2/union_log_cfg/log_file_1.log", TRACE).
                                add_stream("stdout", TRACE).create();
    logger *logger_2 = builder->add_stream(root["streams"][0]["name"].asString(), static_cast<severity_t>(root["streams"][0]["severity"].asInt())).
                                add_stream(root["streams"][1]["name"].asString(), static_cast<severity_t>(root["streams"][1]["severity"].asInt())).
                                add_stream(root["streams"][2]["name"].asString(), static_cast<severity_t>(root["streams"][2]["severity"].asInt())).
                                add_stream(root["streams"][3]["name"].asString(), static_cast<severity_t>(root["streams"][3]["severity"].asInt())).
                                add_stream("stdout", INFORMATION).create();
    delete builder;

    logger_1->write_log("log record trace 1", get_date_time(), TRACE);
    logger_1->write_log("log record information 2", get_date_time(), INFORMATION);
    logger_1->write_log("log record trace 3", get_date_time(), DEBUG);
    logger_1->write_log("log record trace 8", get_date_time(), DEBUG);

    logger_2->write_log("log record trace 4", get_date_time(), FATAL);
    logger_2->write_log("log record information 5", get_date_time(), INFORMATION);
    logger_2->write_log("log record trace 6", get_date_time(), DEBUG);
    logger_2->write_log("log record trace 7", get_date_time(), TRACE);

    delete logger_1;
    delete logger_2;
    return 0;
}
