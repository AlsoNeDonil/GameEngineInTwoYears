--sandbox

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"/src/**.h",
		"/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/GameEngineInTwoYears/vendor/spdlog/include",
		"%{wks.location}/GameEngineInTwoYears/src",
		"%{wks.location}/GameEngineInTwoYears/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GameEngineInTwoYears",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize "on"