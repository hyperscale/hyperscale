package main

import (
	"html/template"
	"os"
	"regexp"
	"runtime"
	"strings"
	"time"

	"github.com/coreos/go-semver/semver"
)

var gitRevPattern = regexp.MustCompile(`(?m)STABLE_BUILD_GIT_REV\s(.*)`)
var gitTagPattern = regexp.MustCompile(`(?m)STABLE_BUILD_GIT_DESCRIBE\s(.*)`)

type ConfigInfo struct {
	Version     *semver.Version
	Revision    string
	Channel     string
	CurrentYear string
	OS          string
	Arch        string
}

func main() {
	status := os.Args[1]
	tmplFile := os.Args[2]

	info := &ConfigInfo{
		Version:     &semver.Version{},
		Channel:     "dev",
		CurrentYear: time.Now().UTC().Format("2006"),
		OS:          runtime.GOOS,
		Arch:        runtime.GOARCH,
	}

	content, err := os.ReadFile(status)
	if err != nil {
		panic(err)
	}

	c := string(content)

	info.Revision = gitRevPattern.FindStringSubmatch(c)[1]

	tag := gitTagPattern.FindStringSubmatch(c)[1]

	if strings.Contains(tag, ".") {
		// parse version
		v, err := semver.NewVersion(strings.TrimLeft(tag, "v"))
		if err != nil {
			panic(err)
		}

		info.Version = v
	}

	tmpl, err := template.New("config.hpp.in").ParseFiles(tmplFile)
	if err != nil {
		panic(err)
	}

	err = tmpl.Execute(os.Stdout, info)
	if err != nil {
		panic(err)
	}

	os.Exit(0)
}
