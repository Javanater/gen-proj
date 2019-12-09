//TODO:
// Create CMakeLists.txt - done
// Create main.cpp - done
// Create LICENSE - done
// init git
// add convenience submodule
// commit git
// create out of source build
// optional licenses
// install system wide?
// aur package

#include <using_convenience.hpp>
#include "project-template-files.hpp"

void generate_project(string project_name)
{
	fusion::for_each(embedded_file::files, [&](auto t)
	{
		string& file_path = get<0>(t);
		string content{get<1>(t).data(), get<1>(t).size()};
		replace_all(file_path, "PROJ_NME", project_name);
		create_directories(absolute(path(file_path)).parent_path());
		ofstream ofs(file_path, ios::binary | ios::trunc);

		if (!ofs)
		{
			cerr << "Failed to open %1% for writing\n"_f % file_path;
			return;
		}

		replace_all(content, "PROJ_NME", project_name);
		ofs << content;
	});
}

auto parse_commandline(int argc, char** argv)
{
	optional<string> project_name;
	options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("project-name", po::value(&project_name),
			"The name of the project");

	positional_options_description p;
	p.add("project-name", 1);

	variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).
			options(desc).positional(p).run(), vm);
		po::notify(vm);
	}
	catch (exception const& e)
	{
		throw runtime_error(str("%1%\n%2%"_f % e.what() % desc));
	}

	if (!project_name)
		throw runtime_error(str("No project name specified.\n%1%"_f % desc));

	return make_tuple(*project_name);
}

template<class... Args>
void execute(string command, Args... args)
{
    int return_code = system(search_path(command), args..., bp::std_out > stdout,
            bp::std_err > stderr, bp::std_in < stdin);

    if (return_code != 0)
        throw runtime_error(str("Error: %1% returned %2%"_f % command % return_code));
}

int main(int argc, char** argv)
{
	try
	{
        auto params = parse_commandline(argc, argv);
		unpack(generate_project)(params);
        current_path(get<0>(params));
        execute("git", "init");
        execute("git", "submodule", "add", "git@github.com:Javanater/convenience.git");
        execute("git", "add", ".");
        execute("git", "commit", "-m", "'Auto generated project'");
	}
	catch (exception const& e)
	{
		cerr << e.what();
		return -1;
	}

	return 0;
}
