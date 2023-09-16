#include "stdafx.h"
#include "SkipCount.h"

namespace foo_skipcount {
	class my_library_callback : public library_callback {
	public:
		void on_items_added(metadb_handle_list_cref) final {}
		void on_items_modified(metadb_handle_list_cref) final {}
		void on_items_removed(metadb_handle_list_cref) final {}
	};

	class my_initquit : public initquit {
	public:
		void on_init() final {
			FB2K_console_formatter() << COMPONENT_NAME ": loaded";
		}
		void on_quit() final {
			foobarQuitting();
			FB2K_console_formatter() << COMPONENT_NAME ": unloading";
		}
	};

	class my_init_stage_callback : public init_stage_callback {
	public:
		void on_init_stage(t_uint32 stage) {
			switch(stage) {
				case init_stages::before_config_read:
					addMetadbIndexes();
					break;
			}
		}
	};

	FB2K_SERVICE_FACTORY(my_library_callback);
	FB2K_SERVICE_FACTORY(my_initquit);
	FB2K_SERVICE_FACTORY(my_init_stage_callback);
} // namespace foo_skipcount
