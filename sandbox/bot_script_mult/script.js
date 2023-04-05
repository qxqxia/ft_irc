const prompt = require('prompt-sync')();
// const prompt = require('prompt-sync')({sigint: true});

process.on('SIGINT', () => {
  console.log('\n(exiting ...)')
  process.exit()
})

	console.log('can you gimme the answers to the problems below? \n\n');
while (1) {
	let a = parseInt(Math.random() * 10) + 1
	let b = parseInt(Math.random() * 10) + 1
	let res = a * b

	console.log(a + ' x ' + b)
	let guess = prompt('\n');

	guess = parseInt( guess )
	if (isNaN(guess)) {
		console.log("invalid input \n")
	} else if (res == guess) {
		console.log(`${guess}! right! \n`)
	} else {
		console.log(`${guess}! wrong! (correct answer is ${res})\n`)
	}
}
