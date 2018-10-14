#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H
#include <string>
#include <vector>

class MessageInfo
{
public:
    MessageInfo();
    virtual ~MessageInfo();
private:
    std::string task_id_;
    std::string data_type_;
    std::string search_result_;
    std::string score_;
    std::string black_id_;
public:
    void setTaskID(const std::string& task_id)
    {
        task_id_ = task_id;
    }

    std::string getTaskID()const
    {
        return task_id_;
    }

    void setDataType(const std::string& data_type)
    {
        data_type_ = data_type;
    }

    std::string getDataType()const
    {
        return data_type_;
    }

    void setSearchResult(const std::string& res)
    {
        search_result_ = res;
    }

    std::string getSearchResult()const
    {
        return search_result_;
    }

    void setScore(const std::string& score)
    {
        score_ = score;
    }

    std::string getScore()const
    {
        return score_;
    }

    void setBlackID(const std::string& id)
    {
        black_id_ = id;
    }

    std::string getBlackID()const
    {
        return black_id_;
    }
};





#endif // MESSAGEINFO_H
