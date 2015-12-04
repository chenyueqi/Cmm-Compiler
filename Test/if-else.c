int if_else(int n) {
	int cost;
	if(n > 500) cost = 150;
	else if(n > 300) cost = 100;
	else if(n > 100) cost = 75;
	else if(n > 50) cost = 50;
	else cost = 0;

	return cost;
}


/*
int ans[] = {0, 0, 0, 0, 50, 50, 50, 75, 75, 75, 100, 100, 100, 150};
*/

int main() {
	int NR_DATA = 14;
	int i;
	int test_data[14];
	test_data[0] = -1;
	test_data[1] = 0;
	test_data[2] = 49;
	test_data[3] = 50;
	test_data[4] = 51;
	test_data[5] = 99;
	test_data[6] = 100;
	test_data[7] = 101;
	test_data[8] = 299;
	test_data[9] = 300;
	test_data[10] = 301;
	test_data[11] = 499;
	test_data[12] = 500;
	test_data[13] = 501;

	i = 0;
	while(i < NR_DATA) {
		put_int(if_else(test_data[i]));
		i = i + 1;
	}

	return 0;
}
