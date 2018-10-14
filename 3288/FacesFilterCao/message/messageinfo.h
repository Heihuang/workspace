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
    std::string msg_data_;
    std::vector<unsigned char> blob_data_;
    std::string blob_data_size_;
    std::vector<unsigned char> blob_faceinfo_;
    std::string blob_faceinfo_size_;
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

    void setMsgData(const std::string& msg_data)
    {
        msg_data_ = msg_data;
    }

    std::string getMsgData()const
    {
        return msg_data_;
    }

    void setBlobData(const std::vector<unsigned char>& blob_data)
    {
        blob_data_ = blob_data;
    }

    std::vector<unsigned char> getBlobData()const
    {
        return blob_data_;
    }

    void setBlobDataSize(const std::string& blob_data_size)
    {
        blob_data_size_ = blob_data_size;
    }
    std::string getBlobDataSize()const
    {
        return blob_data_size_;
    }

    void setBlobFaceInfo(const std::vector<unsigned char> blobex)
    {
        blob_faceinfo_ = blobex;
    }

    std::vector<unsigned char> getBlobFaceInfo()const
    {
        return blob_faceinfo_;
    }

    void setBlobFaceInfoSize(const std::string & infosize)
    {
        blob_faceinfo_size_ = infosize;
    }

    std::string getBlobFaceInfoSize()const
    {
        return blob_faceinfo_size_;
    }
};





#endif // MESSAGEINFO_H
