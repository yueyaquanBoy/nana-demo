#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/filesystem/filesystem.hpp>

#if defined(NANA_WINDOWS)
	constexpr auto root = L"C:";
	constexpr auto rootstr = L"C:\\";
	constexpr auto rootname = L"Local Drive(C:)";
#elif defined(NANA_LINUX)
	constexpr auto root = L"/";
	constexpr auto rootstr = L"/";
	constexpr auto rootname = L"Root/";
#endif


int main()
{
	using namespace nana;
	using namespace experimental;
	using SubDirectories = filesystem::directory_iterator;

	form fm{ API::make_center(400, 500), appear::decorate<appear::taskbar>() };
	fm.caption(L"Nana C++ Library - Treebox-nana::filesystem example.");

	nana::treebox tree{ fm, { 10, 10, 380, 480 } };
    
	auto node = tree.insert(root, rootname);

	for ( const auto& dir : SubDirectories{ rootstr })
	{
		if (!filesystem::is_directory(dir)) continue;
		tree.insert(node, dir.path().filename(), dir.path().filename());
		break;
	}

	tree.events().expanded([&tree](const arg_treebox& arg)
	{
		if (!arg.operated) return; //If this is contracted.

		//Windows supports the path separator '/'
		auto Path = tree.make_key_path(arg.item, L"/") + L"/";

		//avoids frequent useless refreshing
		tree.auto_draw(false);

		//Walk in the path directory for sub directories.
		for (const auto& dir : SubDirectories{ Path } )
		{
			if (!filesystem::is_directory(dir)) continue; //If it is not a directory.

			auto child = tree.insert(arg.item, dir.path().filename(), dir.path().filename());
			if (child.empty()) continue;

			//Find a directory in child directory, if there is a directory,
			//insert it into the child, just insert one node to indicate the
			//node has a child and an arrow symbol will be?displayed in the
			//front of the node.
			for (const auto& dr : SubDirectories{ Path + dir.path().filename() })
			{
				if (!filesystem::is_directory(dr)) continue; //If it is not a directory.
				tree.insert(child, dr.path().filename(), dr.path().filename());
				break;
			}
		}
		tree.auto_draw(true);
	});

	fm.show();
	exec();
}