#include "stdafx.h"
#include "SkipCount.h"

namespace foo_skipcount {
	class myLibraryCallbacks : public library_callback {
	public:
		void on_items_added(metadb_handle_list_cref) final {}
		void on_items_modified(metadb_handle_list_cref) final {}
		void on_items_removed(metadb_handle_list_cref) final {}
	};

	class myinitquit : public initquit {
	public:
		void on_init() final {
			console::print(COMPONENT_NAME ": loaded");
		}
		void on_quit() final {
			foobarQuitting();
			console::print(COMPONENT_NAME ": unloading");
		}
	};

	class myInitStageCallback : public init_stage_callback
	{
	public:
		void on_init_stage(t_uint32 stage) {
			switch(stage) {
				case init_stages::before_config_read:
					addMetadbIndexes();
					break;
			}
		}
	};

	FB2K_SERVICE_FACTORY(myLibraryCallbacks);
	FB2K_SERVICE_FACTORY(myinitquit);
	FB2K_SERVICE_FACTORY(myInitStageCallback);
}
