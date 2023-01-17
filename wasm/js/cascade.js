function getQueryParameters(url) {
	const parameterArray = url.slice(url.indexOf('?') + 1).split('&');
	const parameters = {};
	parameterArray.map(parameter => {
		const [key, value] = parameter.split('=');
		parameters[key] = decodeURIComponent(value);
	});
	return parameters;
}

function LoadFile(readme) {
	axios.get(readme).then(response => {
		console.log(response.data);
		document.querySelector('#md').mdContent = response.data;
	}).catch(error => {
		console.log(error);
	});
}

function SetBody(body_url,readme) {
	axios.get(body_url).then(response => {
		console.log(response.data);
		document.body.insertAdjacentHTML('afterbegin', response.data);
		if(readme)
		{
			LoadFile(readme);
		}
	}).catch(error => {
		console.log(error);
	});
}

function Setscripts(scripts_url) {
	axios.get(scripts_url).then(response => {
		createscripts(response.data);
	}).catch(error => {
		console.log(error);
	});
}

function fillScript(i, scripts) {
	oldScript = scripts[i];
	const newScript = document.createElement('script');
	newScript.setAttribute('async', false);
	for (const attr of Array.from(oldScript.attributes)) {
		newScript.setAttribute(attr.name, attr.value);
	}

	newScript.append(document.createTextNode(oldScript.innerHTML));
	      document.body.append(newScript);
	i += 1;
	      newScript.addEventListener('load', () => {
		      if (i < scripts.length) {
			      fillScript(i, scripts);
		      }
	});
}

function createscripts(html) {
	var div = document.createElement('div');
	div.innerHTML = html
	const scripts = div.querySelectorAll('script');

	if (scripts.length > 0) {
		fillScript(0, scripts);
	}
}
function fillfooter(github_repo)
{
	var foot = document.getElementById("gitrepo");
foot.insertAdjacentHTML('afterbegin', '<a style="display: contents; width:30px;" href="'+ github_repo+ '" target="_blank"><img style="width:30px;" id="gitHublogo" src="img/gitHublogo.png"></img></a>');	

}
/*function init() {
	const url = window.location.href;
	console.log(url);
	const parameter = getQueryParameters(url);
	const readme = parameter.readme_url;
	console.log(readme);
	const after_body = parameter.after_body_url;
	const initScripts = parameter.init_scripts;
	const githubrepo = parameter.github_repo;

	if (initScripts) {
		Setscripts(initScripts);
	}

	if (after_body) {
		SetBody(after_body);
	}

	if(githubrepo)	
	{
		fillfooter(githubrepo);
	}

}
*/
