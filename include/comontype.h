
#ifndef __COMONTYPE_H_
#define __COMONTYPE_H_

/*��������ģ��*/

#define IN
#define OUT
#define INOUT

#define RESULT_OK                      0x00000000
#define DEFAULT_ERROR                  0x00000001
#define PARAM_ERROR                    0x00000002
#define MALLOC_ERROR                   0x00000003
#define DATA_ERROR                     0x00000004
#define FILE_NOTEXIST_ERROR            0x00000005
#define FILE_OPEN_ERROR                0x00000006
#define ENGINE_NOTEXIST_ERROR          0x00000007
#define REPEAT_INIT_ERROR              0x00000008                 //�ظ���ʼ��
#define MANAGER_IDLE_ERROR             0x00000009                 //managerδ��ʼ��
#define THREAD_RUN_ERROR               0x0000000A                 //�߳���������
#define TIMEOUT_ERROR                  0x0000000B                 //��ʱ
#define LOAD_LOG_ERROR                 0x0000000C                 //��ʱ

#ifndef UINT
#define UINT unsigned int
#endif

#define LINE_LEN 1024
#define NUMLEN 10

#define ME_FREE(x) if((x)){ free((x)); (x) = NULL; }
#define FE_FREE(x) if((x)){ fclose((x)); (x) = NULL; }
#define DE_FREE(x) if((x)){ delete (x); (x) = NULL; }


#endif


