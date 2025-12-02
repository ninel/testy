#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <zip.h>

using namespace std;

long num = 1, prvn = 1; long mod6[] = {0, 0, 0, 0, 0, 0};
long mod30[] = {0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0};
long mod210[210]; long mod2310[2310], mod30030[30030];
const int MAXGAPS = 10000;
long gaps[MAXGAPS][50];
long fgap[MAXGAPS];

void countGaps(int i)
{
	mod6[num%6]++; mod30[num%30]++; mod210[num%210]++; mod2310[num%2310]++; mod30030[num%30030]++;
	switch(num%6) { case 0: case 2: case 4: cerr << "Even6: " << num << "\n"; }
	int n30 = num%30; if (n30%2==0) { cerr << "Even30: " << num << "\n"; }
	if (num>prvn && num-prvn<MAXGAPS)
	{
		if (fgap[num-prvn]==0) { fgap[num-prvn]=prvn; }
		if (i>=0 && i<50) gaps[num-prvn][i]++;
		gaps[num-prvn][0]++;
	}
	prvn = num;
}

int main(int argc, char** argv)
{
	for (int i=0;i<210; i++) { mod210[i]=0; }
	for (int i=0;i<2310; i++) { mod2310[i]=0; }
	for (int i=0;i<30030; i++) { mod30030[i]=0; }
	for (int i=0;i<MAXGAPS; i++) { fgap[i]=0; for(int j=0;j<50;j++)gaps[i][j]=0;}

	for (int ia=1;ia<argc;ia++)
	{
		std::ifstream ifs(argv[ia]);
		if (!ifs.is_open())
		{
			std::cerr << "Can't open " << argv[ia] << "\n";
		}
		else
			std::cerr << "--Reading " << argv[ia] << "\n";
		if (argv[ia][strlen(argv[ia])-1]=='p')
		{
			cerr << "Zip'd file: " << argv[ia] << "\n";
			zip * archive = zip_open(argv[ia], 0, NULL);
			if (!archive)
			{
				cerr << "Failed to open " << argv[ia] << "\n";
				return 1;
			}
			int numFiles = zip_get_num_files(archive);
		
			for(int i=0; i<numFiles;i++) {
				struct zip_stat fileInfo;
				zip_stat_init(&fileInfo);

				if (zip_stat_index(archive, i, 0, &fileInfo)== 0) {
            std::cout << "File Name: " << fileInfo.name
                      << std::endl;

            // Step 4: Extract and print file contents
            zip_file* file = zip_fopen_index(archive, i, 0);
	    std::stringstream ss; //int iters=0;
            if (file) {
                char buffer[1024];
                while (int zr = zip_fread(file, buffer, sizeof(buffer)) > 0) 
		{
			char *parensclose = strstr(buffer, ")");
			if (parensclose)
			{
				ss << (parensclose+1); cerr << "Trimmed 1st line: [" << parensclose+1 << "]\n";
			}
			else
			{ //buffer[zr]=0;
				ss << buffer; //buffer[10]=0; //if (iters++<2) 
				cerr << "[" << std::string(buffer).substr(0,10) << ".." << buffer+zr-10 << "] " << ss.str().size() << "\n"; //<< "SS:[" << ss.str() << "]\n";
			}
                    //std::cout.write( buffer, zip_fread(file, buffer, sizeof(buffer)));
                }
                zip_fclose(file);
		while (ss >> num) 
		{ //std::cout << num << "\n";
			countGaps(ia);
		}
            }
        }
			}
			zip_close(archive);
			continue;
		}
		//string line; while (getline(ifs, line)) { std::cout << line << "\n"; }
		string line; getline(ifs, line);
		while (ifs >> num) { //std::cout << num << "\n";
			countGaps(ia);
		}
		ifs.close();
	}

	cout << "----6\n"; for (int i=0;i<6; i++) { if (mod6[i]>0) cout << 6 << ":" << i << " : " << mod6[i] << "\n"; }
	cout << "----30\n"; for (int i=0;i<30; i++) { if (mod30[i]>0) cout << 30 << ":" << i << " : " << mod30[i] << "\n"; }
	cout << "----210\n"; for (int i=0;i<210; i++) { if (mod210[i]>0) cout << 210 << ":" << i << " : " << mod210[i] << "\n"; }
	cout << "----2310\n"; for (int i=0;i<2310; i++) { if (mod2310[i]>0) cout << 2310 << ":" << i << " : " << mod2310[i] << "\n"; }
	cout << "----30030\n"; for (int i=0;i<30030; i++) { if (mod30030[i]>0) cout << 30030 << ":" << i << " : " << mod30030[i] << "\n"; }
	cout << "----fgap\n"; for (int i=0;i<MAXGAPS; i++) { if (fgap[i]>0) cout << "fgap:" << i << " : " << fgap[i] << "\n"; }
	cout << "----gaps\n"; for (int i=0;i<MAXGAPS; i++) { if (gaps[i][0]>0) cout << "gaps:" << i << " : " << gaps[i][0] << "\n"; }
	cout << "----gaps2\n"; for (int i=6;i<MAXGAPS; i=i+6) { if (gaps[i][0]>0) cout << "gaps:" << i; for(int j=1;j<50;j++) cout << ":" << j << " : " << gaps[i][j] << "\n"; }

	return 0;
}
