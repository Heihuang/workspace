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
    std::string face_found_;
    std::vector<unsigned char> blob_data_;
	std::vector<unsigned char> fullview_data_;
    std::string blob_data_size_;
    std::vector<unsigned char> blob_faceinfo_;
    std::string blob_faceinfo_size_;
    std::string face_score_;
    std::string yaw_; //
    std::string pitch_;
    std::string roll_;
    std::string faceid_;
    std::string face_width_;
    std::string eye_width_;
    std::string brightness_;
    std::string definition_;
    std::string ipc_id_;
    std::string cur_time_;
    std::string dev_ID_;
	std::string facequalityscore_;
	std::string facesize_;
	std::string talk_id_;
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

	void setFullViewData(const std::vector<unsigned char>& fullview_data)
	{
		fullview_data_ = fullview_data;
	}

	std::vector<unsigned char> getFullViewData()const
	{
		return fullview_data_;
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

    void setFaceFound(const std::string& facefound)
    {
        face_found_ = facefound;
    }

    std::string getFaceFound()const
    {
        return face_found_;
    }

    void setFaceScore(const std::string& facescore)
    {
        face_score_ = facescore;
    }

    std::string getFaceScore()const
    {
        return face_score_;
    }

    void setYaw(const std::string& yaw)
    {
        yaw_ = yaw;
    }

    std::string getYaw()const
    {
        return yaw_;
    }

    void setPitch(const std::string& pitch)
    {
        pitch_ = pitch;
    }

    std::string getPitch()const
    {
        return pitch_;
    }

    void setRoll(const std::string& roll)
    {
        roll_ = roll;
    }

    std::string getRoll()const
    {
        return roll_;
    }

    void setFaceId(const std::string& faceid)
    {
        faceid_ = faceid;
    }

    std::string getFaceId()const
    {
        return faceid_;
    }

    void setFaceWidth(const std::string& facewidth)
    {
        face_width_ = facewidth;
    }

    std::string getFaceWidth()const
    {
        return face_width_;
    }

    void setEyeWidth(const std::string& eyewidth)
    {
        eye_width_ = eyewidth;
    }

    std::string getEyeWidth()const
    {
        return eye_width_;
    }

    void setBrightness(const std::string& brightness)
    {
        brightness_ = brightness;
    }

    std::string getBrightness()const
    {
        return brightness_;
    }

    void setDefinition(const std::string& definition)
    {
        definition_ = definition;
    }

    std::string getDefinition()const
    {
        return definition_;
    }

    void setIpcID(const std::string& ipc_id)
    {
        ipc_id_ = ipc_id;
    }

    std::string getIpcId()const
    {
        return ipc_id_;
    }

    void setCurTime(const std::string& cur_time)
    {
        cur_time_ = cur_time;
    }

    std::string getCurTime()const
    {
        return cur_time_;
    }

    void setDevID(const std::string& dev_id)
    {
        dev_ID_  = dev_id;
    }

    std::string getDevID()const
    {
        return dev_ID_;
    }
	void setFaceQualityScore(const std::string& facequalityscore)
	{
		facequalityscore_ = facequalityscore;
	}

	std::string getFaceQualityScore()const
	{
		return facequalityscore_;
	}

	void setFaceSize(const std::string& facesize)
	{
		facesize_ = facesize;
	}

	std::string getFaceSize()const
	{
		return facesize_;
	}

	void setTalkId(const std::string& talkid)
	{
		talk_id_ = talkid;
	}
	std::string getTalkId()const
	{
		return talk_id_;
	}
};





#endif // MESSAGEINFO_H
