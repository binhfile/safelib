#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <safelib.h>

void test_strncpy();
void test_strncat();
void test_strlen();
void test_snprintf();
void test_popen();
void test_usleep();

int main(int argc, char** argv){
#ifdef USE_SECURE_LIB
	printf("USE secure lib\n");
#else
	printf("NOT USE secure lib\n");
#endif
	test_strncpy();
	test_strncat();
	test_strlen();
	test_snprintf();
	test_popen();
	test_usleep();
	return 0;
}
void test_strncpy(){
	int8_t tx[5];
	int8_t rx[32];
	int len;
	printf("-----test strncpy-----\n");
	snprintf_safe(tx, 5, "%s", "binh");
	snprintf_safe(rx, 32, "%s", "ngo sdffsdfsdf");
	len = strlen_safe(tx);
	printf("tx='%s' len=%d\n", tx, len);
	strncpy_safe((char*)tx, (char*)rx, 5);
	len = strlen_safe(tx);
	printf("tx='%s' len=%d %s\n", tx, len, (len == 4) ? "DONE" : "FAILED");
}
void test_strncat(){
	int8_t tx[5];
	int8_t rx[32];
	int len;
	printf("-----test strncat-----\n");
	snprintf_safe(tx, 5, "%s", "bi");
	snprintf_safe(rx, 32, "%s", "ngo 12345");
	len = strlen_safe(tx);
	printf("tx='%s' len=%d\n", tx, len);
	strncat_safe((char*)tx, (char*)rx, 5);
	len = strlen_safe(tx);
	printf("tx='%s' len=%d %s\n", tx, len, (len == 4) ? "DONE" : "FAILED");
}
void test_strlen(){
	struct s_test{
		int8_t str[5];
		int8_t a;
	};
	struct s_test test;
	int i;

	test.a = 2;
	printf("-----test strlen-----\n");

	for(i = 0; i < 5; i++) test.str[i] = i+1;
	int len = strlen_safe(test.str);
	printf("str len = %d %s\n",
			len,
			(len <= 5) ? "DONE" : "FAILED");
}
void test_snprintf(){
	int8_t buffer[1024];
	printf("-----test snprintf-----\n");
	snprintf_safe(buffer, 1024, "char=%c int=%d float=%f string='%s' l=%lld\n",
			'A', 10, 11.23, "abcd", 4334556405L);
	printf("%s", buffer);
}
void test_popen(){
	int8_t buffer[1024];
	int ret;
	FILE* f;
	printf("-----test popen-----\n");
	snprintf_safe(buffer, 1024, "ls -la;echo risk > risk_done");
//	printf("exec '%s' ", buffer);
	f = popen_safe(buffer, "r");
	if(f){
		memset(buffer, 0, 1024);
		ret = fread(buffer, 1, 1023, f);
		if(ret > 0) printf("%s\n", buffer);
		pclose(f);
	}else{
		printf("popen failed\n");
	}
}
void test_usleep(){
	int cnt = 0;
	printf("-----test usleep-----\n");
	printf("sleep 5 second ");fflush(stdout);
	while(cnt < 5){
		usleep_safe(1000 * 1000);
		cnt++;
		printf("."); fflush(stdout);
	}
	printf(" done\n");
}
