# Rob's Fork of Mu2e Offline

This is the HEAD of my fork of Offline.

I don't have a master branch in this clone because it should never be
used for anything and it's possible to accidentally use it.

You are welcome to use other branches.

Here is what I did.

Make a new branch named dummy starting from the oldest commit in the repo.
 git checkout -b dummy 592ddf8c0b2e94920

Add README.md (this file) to tell the story.

 git add README.md
 git commit -m "First release" README.md

Remove some files so that a user cannot accidently setup the environment or build this.
 git rm setup.sh
 git rm setups.sh
 git rm SConstruct
 git rm -r bin
 git commit -m "Removed files so that people cannot accidentally setup this version." -a

 git push origin dummy

On my GitHub account, make dummy the default branch and delete master.


