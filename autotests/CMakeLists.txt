
if(KDE4_BUILD_TESTS)
    # Add Tests
    add_subdirectory( Pipe.movie )
    add_subdirectory( Pipe.music )
    add_subdirectory( Pipe.publication )
    add_subdirectory( Pipe.tvshow )

    add_subdirectory( Extractor.filename )

endif()
