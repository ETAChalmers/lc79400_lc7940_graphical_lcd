void _start(void);
void SystemInit(void);
int main(void);

void _start(void) {
	SystemInit();
	main();
	while(1) {
	}
}
