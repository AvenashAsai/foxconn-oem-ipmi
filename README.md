# Purpose

The purpose of this repository is for implementations of Foxconn OEM IPMI commands in OpenBMC projects.

# Add to a Project
Follow these steps, to build this repository into a project:
1. Copy the obmc-phosphor-image.bbappend from /home/boyer/ on the OpenBMC build server into the recipes-phosphor/images/ directory of the project.
2. Copy the foxconn-oem-ipmi_git.bb from /home/boyer into the recipes-phoshpor/ipmi/ directory of the project.
3. Attempt to clone the foxconn-oem-ipmi repository on the build machine to register known_hosts.
4. Register the SSH key of the build machine with your Gitlab account settings.
5. Attempt to build the project, if it completes, then change SRCREV in foxconn-oem-ipmi_git.bb to the commit SHA of the common-dev branch revision to build.

Note: Step 3 can be skipped if the Gitlab server is already a known host of the build machine user.

Note: Step 4 can be skipped if the Gitlab server already has a copy of the public SSH key of the build machine user.

# Development
Development of the repository should be done with a development workspace, and note the different branches and their purpose(s).

## Branches
common-dev: Branch with development code for common IPMI commands

master: Branch with stable, common code for releases

tomahawk-dev: Branch that can be created for Tomahawk platform-specific commands.

To push local changes to the remote repository, navigate to the local workspace repository at /build/workspace/sources/foxconn-oem-ipmi, and then do the following:
1. git status (to check status of changes to commit)
2. git commit -m "Commit message"
3. git push (to the branch to push changes)

## Setup Development Workspace
Follow these steps to create a development workspace:

1. Navigate to the openbmc project repository.
2. Run the command, ". openbmc-env" to enter the openbmc environment.
3. Build the openbmc project, so that a "build" directory exists for the project.
4. Run the command, "devtool modify foxconn-oem-ipmi" to build the workspace directory under /build/workspace/sources/foxconn-oem-ipmi.

A .bbappend will be added to the build/appends directory to build changes from the local workspace directory instead of the remote repository for development.

## Remove Development Workspace
Follow these steps to remove the workspace and build from the remote repository:
1. Run "devtool reset foxconn-oem-ipmi"
2. Manually remove the workspace folder with "rm"

