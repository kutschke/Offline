# Rob's Fork of Mu2e Offline

This is the HEAD of my fork of Offline.

I don't have a master branch in this clone because it should never be
used for anything and it's possible to accidentally use it.

You are welcome to use other branches.

Here is what I did.

Make a new branch named dummy starting from the oldest commit in the repo.
<pre>
 git checkout -b dummy 592ddf8c0b2e94920
</pre?

Add README.md (this file) to tell the story.

<pre>
 git add README.md
 git commit -m "First release" README.md
</pre>

Remove some files so that a user cannot accidently setup the environment or build this.
<pre>
 git rm setup.sh
 git rm setups.sh
 git rm SConstruct
 git rm -r bin
 git commit -m "Removed files so that people cannot accidentally setup this version." -a
</pre>

Push this branch back to the GitHub fork:

<pre>
 git push origin dummy
</pre>

On my GitHub account, use the GUI to make dummy the default branch and delete master.



