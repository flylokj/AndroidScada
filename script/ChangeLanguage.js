x=readIntVar('$317');
if(x==1)
{
	writeVar('$317', '0');
	x=0;
}
else
{
	writeVar('$317', '1');
	x=1;
}
changeLanguage(x);