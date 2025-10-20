@echo on
git checkout <stable>
git pull origin <stable>
git merge <development>
git merge <test>
git push origin <stable>
pause
