# Rob's Fork of Mu2e Offline

This is the HEAD branch of my fork of Mu2e Offline https://github.com/Mu2e/Offline .

I have removed the master branch from this fork because it should never be
used for anything and it's possible to accidentally use it.  Always
start new Mu2e projects by branching from the master branch of
https://github.com/Mu2e/Offline


Other branches are working branches.  Some are destined for pull requests
back to the Mu2e Offline repository and some are dead end working branches.

Here is what I did.

Fork the Mu2e Offline repo.  And clone the fork.

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



