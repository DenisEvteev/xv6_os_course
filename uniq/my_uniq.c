#include "types.h"
#include "stat.h"
#include "user.h"

char tolower(unsigned char ch) {
    if (ch >= 'A' && ch <= 'Z')
        ch = 'a' + (ch - 'A');
    return ch;
}

int strcicmp(const char *a, const char *b)
{
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
    }
}


const int GENERAL = 1048576;

struct data {
    int str_beg;
    int repeat;
};

int index_of_zero_symbol(const char *p, int i);
struct data *uniq(int fd, char* buf, int* num, int flag_i);
void print_basic_unique(struct data *info, char *buf);


int main(int argc, char **argv) // argv[1] -- the filename to read input data from
{
    char* buf = malloc(GENERAL);
    if (buf == 0) {
	    printf(2, "Error in allocating memory\n");
	    exit();
    }
    memset(buf, 0, GENERAL);
    
    int flag_i = 0;
    
    struct data *info;
    int lines = 0;
    if (argc == 1) {
        info = uniq(0, buf, &lines, flag_i);
        print_basic_unique(info, buf);
        exit();
    }

    int fd = open(argv[1], 0);
    if (fd < 0) {
        printf(2, "Error in opening file %s\n", argv[1]);
        exit();
    }

    if (argc == 2) {
        info = uniq(fd, buf, &lines, flag_i);
        print_basic_unique(info, buf);
        exit();
    }

    /**
     * We can have two types of double options: -c -i; -d -i.
     * */
    char c;
    argv += 2; // rn this pointer points to the begining of options
    
    //ex: uniq    cool_file  -c       -i     (C-like array the last el is NULL)
    //    argv[0] argv[1]   argv[2] argv[3]
   
    while(argv[0]) {
        if (argv[0][0] == '-')
            break;
	++argv;
    }
    c = *(++argv[0]);
    if(!(c == 'd' || c == 'c' || c == 'i')) {
	printf(2, "Bad option!\n");
	exit();
    }
    argv++;
    if(c == 'i' || (argv && argv[0][0] == '-' && argv[0][1] == 'i'))
	    flag_i = 1;
    info = uniq(fd, buf, &lines, flag_i);
    if (c == 'i') {
	    if(argv && argv[0][0] == '-' && (argv[0][1] == 'c' || argv[0][1] == 'd'))
		    c = argv[0][1];
    }
    
    switch (c) {
	    case 'c' : {
		int i = 0;
                while (i < lines) {
                   	printf(1, "%d %s\n", info[i].repeat, buf + info[i].str_beg);
                    	++i;
		}
                break;
            }
            case 'd' : {
                int i = 0;
                while (i < lines) {
                    if(info[i].repeat >= 2)
                        printf(1, "%s\n", buf + info[i].str_beg);
                    ++i;
                }
                break;
	    }
            case 'i' :
                print_basic_unique(info, buf);
                break;
	    default:
		printf(2, "Weird case, some strange letter");
		break;
    }
    free(info);
    close(fd);
    free(buf);
    exit();
}

void print_basic_unique(struct data *info, char *buf) 
{
    int i = 0;
    while (info && info[i].repeat != 0) {
	printf(1, "%s\n", buf + info[i].str_beg);
       	++i;
    }
    free(info);
}

struct data *uniq(int fd, char *buf, int* num, int flag_i)
{
    int res = read(fd, buf, GENERAL);
    if (res < 0) {
        printf(2, "Error in reading data\n");
        exit();
    }
    int nstr = 0;
    for (int i = 0; i < res; ++i) {
        if (buf[i] == '\n') {
        	buf[i] = '\0';
            nstr++;
        }	
    }
    buf[res] = '\0';
    nstr++;
    
    struct data *info = malloc(nstr * sizeof(struct data));
    if (info == 0) {
   	printf(2, "Error in allocating memory\n");
   	exit();
    }
    memset(info, 0, nstr * sizeof(struct data));
        
    int index = index_of_zero_symbol(buf, 0);
    int string_number = 0;
        
    info[string_number].repeat = 1;

    int counter = 0;//number of processed lines
    int j = 0;
    ++index; // currently this value represents the first letter of new string

    while (counter != nstr - 1) {
        if ((flag_i == 0 ? strcmp(buf + j, buf + index) : strcicmp(buf + j, buf + index)) != 0) {
        	++string_number;
           	info[string_number].str_beg = index;
           	info[string_number].repeat = 1;
            	j = index;
        } else {
            info[string_number].repeat++;
        }
    	index = index_of_zero_symbol(buf + index, index) + 1;
        counter++;
    }
    *num = string_number + 1;
    return info;
}

int index_of_zero_symbol(const char *p, int i)
{
        if (!p)
                return -1;
        while (p) {
                if (*p == '\0')
                        return i;
                ++i;
                ++p;
        }
        return -1;
}
