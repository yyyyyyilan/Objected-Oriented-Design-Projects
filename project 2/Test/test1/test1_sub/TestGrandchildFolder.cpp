template <typename T>
class FilesInSubfolder1
{
public:
	FilesInSubfolder1();
	void Test();
private:
	size_t i;
}

template <typename T>
FilesInSubfolder1<T>::FilesInSubfolder1() { i = 1; }

template <typename T>
FilesInSubfolder1<T>::Test()
{
	std::cout << "This file tests filemgr finding files in subfolder." << std::endl;
}

template<typename T>
auto glambda = [](auto a) { return a; };

NC = [](auto a) {static int s; return a; };
