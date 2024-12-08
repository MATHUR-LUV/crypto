document.getElementById('analyzeBtn').addEventListener('click', function () {
    const cipherSelect = document.getElementById('cipherSelect').value;
    const ciphertext = document.getElementById('ciphertext').value;
    const output = document.getElementById('output');

    if (ciphertext.trim() === "") {
        output.textContent = "Please enter the ciphertext.";
        return;
    }

    switch (cipherSelect) {
        case "caesar":
            output.textContent = caesarBruteForce(ciphertext);
            break;
        case "substitution":
            output.textContent = substitutionFrequencyAnalysis(ciphertext);
            break;
        case "vigenere-manual":
            const vigenereKey = prompt("Enter the Vigenère key:");
            output.textContent = vigenereDecrypt(ciphertext, vigenereKey);
            // output.textContent = vigenereKasinski(ciphertext);
            break;
        case "vigenere-kasinski":
            output.textContent = vigenereKasinski(ciphertext);
            break; 
        case "affine":
            const affineA = parseInt(prompt("Enter value for 'a':"));
            const affineB = parseInt(prompt("Enter value for 'b':"));
            output.textContent = affineDecrypt(ciphertext, affineA, affineB);
            break;
        case "playfair":
            const playfairKey = prompt("Enter the Playfair key:");
            output.textContent = playfairDecrypt(ciphertext, playfairKey);
            break;
        default:
            output.textContent = "Invalid cipher selected.";
            break;
    }
});


function caesarBruteForce(ciphertext) {
    let result = "Caesar Cipher Brute Force Results:\n";
    for (let key = 0; key < 26; key++) {
        let decrypted = "";
        for (let i = 0; i < ciphertext.length; i++) {
            let char = ciphertext[i];
            if (char.match(/[a-z]/i)) {
                let code = ciphertext.charCodeAt(i);
                if (code >= 65 && code <= 90) {
                    decrypted += String.fromCharCode(((code - 65 + key) % 26) + 65);
                } else if (code >= 97 && code <= 122) {
                    decrypted += String.fromCharCode(((code - 97 + key) % 26) + 97);
                }
            } else {
                decrypted += char;
            }
        }
        result += `Key ${key}: ${decrypted}\n`;
    }
    return result;
}


function substitutionFrequencyAnalysis(ciphertext) {
    const frequency = {};
    const total = ciphertext.length;
    ciphertext.toUpperCase().split('').forEach(char => {
        if (char.match(/[A-Z]/)) {
            frequency[char] = (frequency[char] || 0) + 1;
        }
    });

    let result = "Substitution Cipher Frequency Analysis:\n";
    for (let char in frequency) {
        let percent = ((frequency[char] / total) * 100).toFixed(2);
        result += `${char}: ${frequency[char]} (${percent}%)\n`;
    }

    return result;
}

function vigenereDecrypt(ciphertext, key) {
    let result = "";
    key = key.toUpperCase();
    let keyIndex = 0;

    for (let i = 0; i < ciphertext.length; i++) {
        let char = ciphertext[i];
        if (char.match(/[a-zA-Z]/)) {
            let code = char.charCodeAt(0);
            let offset = char === char.toUpperCase() ? 65 : 97;
            let keyChar = key[keyIndex % key.length].charCodeAt(0) - 65;

            if (code >= 65 && code <= 90) {
                result += String.fromCharCode(((code - 65 - keyChar + 26) % 26) + 65);
            } else if (code >= 97 && code <= 122) {
                result += String.fromCharCode(((code - 97 - keyChar + 26) % 26) + 97);
            }
            keyIndex++;
        } else {
            result += char;
        }
    }

    return result;
}


function affineDecrypt(ciphertext, a, b) {
    let result = "";
    const m = 26;
    const aInverse = modInverse(a, m);
    if (aInverse === -1) {
        return "No modular inverse for a, decryption not possible.";
    }

    for (let i = 0; i < ciphertext.length; i++) {
        let char = ciphertext[i];
        if (char.match(/[a-zA-Z]/)) {
            let code = char.toUpperCase().charCodeAt(0) - 65;
            let decrypted = (aInverse * (code - b + m)) % m;
            if (decrypted < 0) decrypted += m;
            result += String.fromCharCode(decrypted + 65);
        } else {
            result += char;
        }
    }

    return result;
}

// Helper function to find modular inverse of 'a'
function modInverse(a, m) {
    for (let x = 1; x < m; x++) {
        if ((a * x) % m === 1) {
            return x;
        }
    }
    return -1; // No modular inverse exists
}

/* Playfair Cipher Decryption */
function playfairDecrypt(ciphertext, key) {
    let playfairMatrix = generatePlayfairMatrix(key);
    let result = "";
    ciphertext = ciphertext.toUpperCase().replace(/[^A-Z]/g, '');

    for (let i = 0; i < ciphertext.length; i += 2) {
        let pair = ciphertext.substr(i, 2);
        if (pair.length < 2) break;

        let [row1, col1] = findPosition(playfairMatrix, pair[0]);
        let [row2, col2] = findPosition(playfairMatrix, pair[1]);

        if (row1 === row2) {
            result += playfairMatrix[row1][(col1 + 4) % 5] + playfairMatrix[row2][(col2 + 4) % 5];
        } else if (col1 === col2) {
            result += playfairMatrix[(row1 + 4) % 5][col1] + playfairMatrix[(row2 + 4) % 5][col2];
        } else {
            result += playfairMatrix[row1][col2] + playfairMatrix[row2][col1];
        }
    }

    return result;
}

// Helper function to generate Playfair matrix from key
function generatePlayfairMatrix(key) {
    key = key.toUpperCase().replace(/[^A-Z]/g, '').replace('J', 'I');
    let alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    let matrix = [];
    let used = {};

    for (let char of key + alphabet) {
        if (!used[char]) {
            matrix.push(char);
            used[char] = true;
        }
    }

    let playfairMatrix = [];
    for (let i = 0; i < 5; i++) {
        playfairMatrix.push(matrix.slice(i * 5, i * 5 + 5));
    }

    return playfairMatrix;
}

// Helper function to find position of a character in Playfair matrix
function findPosition(matrix, char) {
    for (let row = 0; row < 5; row++) {
        for (let col = 0; col < 5; col++) {
            if (matrix[row][col] === char) {
                return [row, col];
            }
        }
    }
}



// function vigenereDecrypt(ciphertext, key) {
//     let result = "";
//     key = key.toUpperCase(); // Ensure key is uppercase for consistent processing
//     let keyIndex = 0;

//     for (let i = 0; i < ciphertext.length; i++) {
//         let char = ciphertext[i];

//         // Check if the character is alphabetic
//         if (/[a-zA-Z]/.test(char)) {
//             let code = char.charCodeAt(0);
//             let offset = char === char.toUpperCase() ? 65 : 97; // Determine case offset
//             let keyChar = key[keyIndex % key.length].charCodeAt(0) - 65; // Map key character to 0-25

//             // Perform decryption
//             result += String.fromCharCode(((code - offset - keyChar + 26) % 26) + offset);

//             keyIndex++; // Move to the next key character
//         } else {
//             // Append non-alphabetic characters as-is
//             result += char;
//         }
//     }

//     return result;
// }

// // Helper function to find the greatest common divisor (GCD)
// function gcd(a, b) {
//     return b === 0 ? a : gcd(b, a % b);
// }

// // Kasiski Examination to estimate the key length
// function kasiskiExamination(ciphertext) {
//     const nGramSize = 3; // Minimum size of repeated n-grams
//     const distances = [];
//     const nGramPositions = {};

//     // Search for repeated n-grams in the ciphertext
//     for (let i = 0; i <= ciphertext.length - nGramSize; i++) {
//         const nGram = ciphertext.slice(i, i + nGramSize);

//         if (!nGramPositions[nGram]) {
//             nGramPositions[nGram] = [i];
//         } else {
//             nGramPositions[nGram].push(i);
//         }
//     }

//     // Calculate distances between n-gram occurrences
//     for (const nGram in nGramPositions) {
//         const positions = nGramPositions[nGram];
//         if (positions.length > 1) {
//             for (let i = 1; i < positions.length; i++) {
//                 distances.push(positions[i] - positions[i - 1]);
//             }
//         }
//     }

//     // Compute the GCD of all distances
//     if (distances.length === 0) {
//         return "No repeated n-grams found.";
//     }

//     let estimatedKeyLength = distances[0];
//     for (let i = 1; i < distances.length; i++) {
//         estimatedKeyLength = gcd(estimatedKeyLength, distances[i]);
//     }

//     return estimatedKeyLength;
// }

// // // Example Usage
// // const ciphertext = "YMMDLZYMMDLZ"; // Example ciphertext
// // console.log("Estimated Key Length:", kasiskiExamination(ciphertext));


// Helper function to find the greatest common divisor (GCD)
function gcd(a, b) {
    return b === 0 ? a : gcd(b, a % b);
}

// Kasiski Examination for estimating the key length
function kasiskiExamination(ciphertext) {
    const nGramSize = 3; // Minimum size of repeated n-grams
    const distances = [];
    const nGramPositions = {};

    // Find repeated n-grams in the ciphertext
    for (let i = 0; i <= ciphertext.length - nGramSize; i++) {
        const nGram = ciphertext.slice(i, i + nGramSize);

        if (!nGramPositions[nGram]) {
            nGramPositions[nGram] = [i];
        } else {
            nGramPositions[nGram].push(i);
        }
    }

    // Calculate distances between repeated n-grams
    for (const nGram in nGramPositions) {
        const positions = nGramPositions[nGram];
        if (positions.length > 1) {
            for (let i = 1; i < positions.length; i++) {
                distances.push(positions[i] - positions[i - 1]);
            }
        }
    }

    // Compute the GCD of all distances
    if (distances.length === 0) {
        return "No repeated n-grams found.";
    }

    let estimatedKeyLength = distances[0];
    for (let i = 1; i < distances.length; i++) {
        estimatedKeyLength = gcd(estimatedKeyLength, distances[i]);
    }

    return estimatedKeyLength;
}

// Index of Coincidence for estimating the key length
function indexOfCoincidence(ciphertext, maxKeyLength = 40) {
    const frequencies = (text) => {
        const freq = Array(26).fill(0);
        for (const char of text) {
            if (/[A-Z]/.test(char)) freq[char.charCodeAt(0) - 65]++;
        }
        return freq;
    };

    const calculateIC = (group) => {
        const freq = frequencies(group);
        const n = group.length;
        const numerator = freq.reduce((sum, f) => sum + f * (f - 1), 0);
        return numerator / (n * (n - 1));
    };

    ciphertext = ciphertext.toUpperCase().replace(/[^A-Z]/g, ""); // Filter out non-alphabetic characters
    let bestKeyLength = 1;
    let closestIC = 0;

    for (let keyLength = 1; keyLength <= maxKeyLength; keyLength++) {
        const groups = Array.from({ length: keyLength }, () => "");
        for (let i = 0; i < ciphertext.length; i++) {
            groups[i % keyLength] += ciphertext[i];
        }

        const averageIC = groups.reduce((sum, group) => sum + calculateIC(group), 0) / keyLength;

        if (Math.abs(averageIC - 0.068) < Math.abs(closestIC - 0.068)) {
            closestIC = averageIC;
            bestKeyLength = keyLength;
        }
    }

    return bestKeyLength;
}

function vigenereKasinski(ciphertext){
    result="";
    let ans1 = kasiskiExamination(ciphertext);
    // let ans1 = kasiskiAnalysis(ciphertext);
    let ans2 = indexOfCoincidence(ciphertext);

    result += "Kasiski Estimated Key Length: " + ans1;
    result += "\n";
    result += "Index of Coincidence Estimated Key Length: " + ans2;

    return result;
}

// Example Usage
// const ciphertext = "YMMDLZYMMDLZ"; // Example ciphertext

// console.log("Kasiski Estimated Key Length:", kasiskiExamination(ciphertext));
// console.log("Index of Coincidence Estimated Key Length:", indexOfCoincidence(ciphertext));








// Helper function to find repeating sequences in ciphertext
function findRepeatingSequences(ciphertext, sequenceLength = 3) {
    const sequences = {};
    for (let i = 0; i <= ciphertext.length - sequenceLength; i++) {
        const sequence = ciphertext.slice(i, i + sequenceLength);
        if (sequences[sequence]) {
            sequences[sequence].push(i);
        } else {
            sequences[sequence] = [i];
        }
    }

    // Filter only sequences with more than one occurrence
    return Object.values(sequences).filter(indices => indices.length > 1);
}

// Function to calculate GCD of an array of numbers
function gcd(a, b) {
    while (b) {
        let temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Kasiski Examination Method to estimate the key length
function kasiskiExamination(ciphertext, sequenceLength = 3) {
    const repeatingSequences = findRepeatingSequences(ciphertext, sequenceLength);
    const distances = [];

    // Calculate the distance between repeated sequences
    repeatingSequences.forEach(indices => {
        for (let i = 1; i < indices.length; i++) {
            distances.push(indices[i] - indices[i - 1]);
        }
    });

    // Find the GCD of all distances
    let keyLength = distances.reduce((gcdValue, distance) => gcd(gcdValue, distance), distances[0]);
    return keyLength;
}

// Function to guess the key using frequency analysis for each key position
function guessKey(ciphertext, keyLength) {
    const key = [];
    for (let i = 0; i < keyLength; i++) {
        // Extract every ith character from the ciphertext
        const group = [];
        for (let j = i; j < ciphertext.length; j += keyLength) {
            group.push(ciphertext[j]);
        }

        // Perform frequency analysis on the group
        const freq = Array(26).fill(0);
        group.forEach(char => {
            if (/[A-Z]/.test(char)) {
                freq[char.charCodeAt(0) - 65]++;
            }
        });

        // Find the most frequent letter (assuming it's 'E' in English)
        const mostFrequentIndex = freq.indexOf(Math.max(...freq));
        const keyChar = String.fromCharCode((mostFrequentIndex + 65) - 4); // Assuming 'E' is the most frequent letter
        key.push(keyChar);
    }
    return key.join('');
}

// Main function for Kasiski Examination and Key Guessing
function kasiskiAnalysis(ciphertext) {
    // Step 1: Perform Kasiski Examination to find the key length
    const keyLength = kasiskiExamination(ciphertext);
    console.log("Kasiski Estimated Key Length:", keyLength);

    // Step 2: Guess the key based on the estimated key length
    const key = guessKey(ciphertext, keyLength);
    console.log("Guessed Key:", key);
}

// Example Usage
// const ciphertext = "YMMDLZYMMDLZ"; // Example ciphertext

kasiskiAnalysis(ciphertext);