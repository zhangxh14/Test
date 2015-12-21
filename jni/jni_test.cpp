#include <com_example_jniTest_MainActivity.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_INFO, "zxh", __VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_example_jniTest_MainActivity_getMac(JNIEnv* env, jobject obj, jstring prompt){
	  LOGD("########################################################");
	  int sd;
	  struct arpreq arpreq;
	  struct sockaddr_in *sin;
	  struct in_addr ina;
	  char *hw_addr;
	  const char* ip = env->GetStringUTFChars(prompt, NULL);

	  sd = socket(AF_INET, SOCK_DGRAM, 0);
	  LOGD("###################################################sd = %d",sd);
	  if (sd < 0)
	  {
		  LOGD("########################################################1");
		  perror("socket() error\n");
		  exit(1);
	  }
	  LOGD("########################################################2");
	  memset(&arpreq, 0, sizeof(struct arpreq));
	  sin = (struct sockaddr_in *) &arpreq.arp_pa;
	  memset(sin, 0, sizeof(struct sockaddr_in));
	  sin->sin_family = AF_INET;
	  ina.s_addr = inet_addr(ip);
	  memcpy(&sin->sin_addr, (char *)&ina, sizeof(struct in_addr));
	  strcpy(arpreq.arp_dev, "wl0.1"); //wifi热点所用的网卡为wl0.1
	  ioctl(sd, SIOCGARP, &arpreq);
	  printf("\nentry has been successfully retreived\n");
	  hw_addr = (char *) arpreq.arp_ha.sa_data;
	  printf("MAC: %x:%x:%x:%x:%x:%x\n",
	  hw_addr[0], hw_addr[1], hw_addr[2], hw_addr[3], hw_addr[4], hw_addr[5]);
	  LOGD("#####################################################hw_addr : %s ",hw_addr);

	 jclass strClass = env->FindClass("Ljava/lang/String;");
	 jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	 jbyteArray bytes = env->NewByteArray(strlen(hw_addr));
	 env->SetByteArrayRegion(bytes, 0, strlen(hw_addr), (jbyte*)hw_addr);
	 jstring encoding = env->NewStringUTF("utf-8");
	 jstring mac_str =  (jstring)env->NewObject(strClass, ctorID, bytes, encoding);

	 return mac_str;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv *env;
    if(vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_4;
}
