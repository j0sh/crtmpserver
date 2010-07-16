function helloWorldFnc(variant)
{
	if(variant==null||variant==undefined)
		return variant;
	//print('BEFORE\n'+dump(variant,0));
	variant['js_1_null']=null;
	variant['js_2_undefined']=undefined;
	variant['js_3_non_null_non_undefined']='something';
	variant['js_4_array']=new Array("Saab","Volvo","BMW",1,12.3);
	variant.js_5_anon_fnc=function(){
		print('anon fnc');
	};
	variant.js_6_non_anon_fnc=nonAnonFnc;
	variant['js_7_date']=new Date();
	//print('AFTER\n'+dump(variant,0));
	return variant;
}

function nonAnonFnc()
{
	print('non anon fnc');
}

function dump(object, depth){
	if(depth==null||depth==undefined)
		depth=0;
	if(object==null)
		return '(null)';
	if(object==undefined)
		return '(undefined)';
	
	var indent='';
	for(var i=0;i<depth;i++)
		indent+='  ';

	var output='';
	for (var key in object)
	{
		output+=indent+key+': ';
		switch (typeof object[key])
		{
			case 'object':
				if(object[key]==null)
					output+=dump(object[key],depth+1)+'\n';
				else
					output+='\n'+dump(object[key],depth+1);
				break;
			default:
				output+=object[key]+'\n';
				break;
		}
	}
	
	return output;
}


