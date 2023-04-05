const prompt = require('prompt-sync')({sigint: true});

while (1) {
	let a = parseInt(Math.random() * 10) + 1
	let b = parseInt(Math.random() * 10) + 1

	console.log('Can you solve this:', a, 'x', b)
	
	let res = a * b

	let guess = prompt('Can you gimme the answer? ');
	guess = parseInt( guess )
	if (res == guess) {
		console.log(`${guess}! right! \n`)
	} else {
		console.log(`${guess}! wrong! (correct answer is ${res})\n`)
	}
}
