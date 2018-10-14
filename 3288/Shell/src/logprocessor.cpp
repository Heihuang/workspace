#include <stdlib.h>
#include <stdio.h>
#include "logprocessor.h"

//���������־��Ŀ¼��
#define LOGDIR "log"
#define MKDIR "mkdir "LOGDIR

using namespace google;

//����Ϣ������������ļ��� LOG(ERROR)
void SignalHandle(const char* data, int size)
{
	std::string str = std::string(data, size);
	/*
	std::ofstream fs("glog_dump.log",std::ios::app);
	fs<<str;
	fs.close();
	*/
	LOG(ERROR) << str;
	//Ҳ����ֱ�������﷢���ʼ������֪ͨ��������������Ǳ��ص���εģ�ÿ�λص�ֻ���һ�д�����Ϣ������������ļ�¼���ļ���Ҳ��Ҫ>��׷��ģʽ���ɣ����������﷢�ʼ�����Ų��Ǻ��ʺϣ��������ǿ��Ե���һ�� SHELL �� PYTHON �ű������˽ű����� sleep 3�����ң�Ȼ�󽫴�
	//����Ϣͨ���ʼ�����ŷ��ͳ�ȥ�������Ͳ���Ҫ��ؽű���ʱ��Ƶ��ִ�У��˷�Ч���ˡ�
}

//GLOG���ã�
LogProcessor::LogProcessor()
{
	system(MKDIR);
	google::InitGoogleLogging("CVS");

	google::SetStderrLogging(GLOG_INFO); //���ü������ google::INFO ����־ͬʱ�������Ļ
	FLAGS_colorlogtostderr = true;    //�����������Ļ����־��ʾ��Ӧ��ɫ

    //google::SetLogDestination(GLOG_INFO, LOGDIR"/LOG_"); //���� google::INFO �������־�洢·�����ļ���ǰ׺
    //google::SetLogDestination(GLOG_WARNING, LOGDIR"/LOG_");   //���� google::WARNING �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(GLOG_ERROR, LOGDIR"/LOG_");   //���� google::ERROR �������־�洢·�����ļ���ǰ׺

	FLAGS_logbufsecs = 0;        //������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
	FLAGS_max_log_size = 100;  //�����־��СΪ 100MB
	FLAGS_stop_logging_if_full_disk = true;     //�����̱�д��ʱ��ֹͣ��־���
}
