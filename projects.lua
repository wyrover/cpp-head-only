includeexternal ("premake-vs-include.lua")

workspace "cpp-head-only"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end

    

    group "cpp-head-only"    
        matches = os.matchdirs("src/*")
        for i = #matches, 1, -1 do           
            create_console_project(path.getname(matches[i]), "src")
                includedirs
                {
                    "."
                }
        end