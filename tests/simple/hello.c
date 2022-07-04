int main(int argc, char **argv)
{
	int values[] = { 1, 2, 3, 4, 5 };

	return values[argc % 5];
}
