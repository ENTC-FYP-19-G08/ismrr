// Initialize Firebase with your configuration
const firebaseConfig = {
    apiKey: "AIzaSyCvRz-H7MwEp1jJzQyTm7qIZxUsjdu2GSs",  
    authDomain: "robot-receptionist-19.firebaseapp.com",  
    databaseURL: "https://robot-receptionist-19-default-rtdb.asia-southeast1.firebasedatabase.app",  
    projectId: "robot-receptionist-19",  
    storageBucket: "robot-receptionist-19.appspot.com",  
    messagingSenderId: "973044233024",  
    appId: "1:973044233024:web:f978ca75ae807c03e100a5"  
  };
  
firebase.initializeApp(firebaseConfig);

// Reference to your Firebase Realtime Database
const database = firebase.database();
const dbRef = database.ref('/ISMRR/'); // Replace with your database path

// Function to read the value from the database
dbRef.on('value', (snapshot) => {    
    const str = displayObject(snapshot.val());
    console.log(str);
    document.getElementById('dbValue').innerHTML = `<pre>${str}</pre>`;
});

// Function to update the value in the database
function sendTopic(topic) {    
    dbRef.child(topic).set(Date.now());    
}

function displayObject(obj, indent = 0) {
    let result = '';

    // Helper function to generate indentation
    const generateIndent = (count) => '  '.repeat(count);

    for (let key in obj) {
        if (obj.hasOwnProperty(key)) {
            const value = obj[key];

            // Add current key-value pair to result string with proper indentation
            result += generateIndent(indent) + key + ': ';

            if (typeof value === 'object' && value !== null) {
                // If the value is an object, recursively call displayObject with increased indentation
                result += '\n' + displayObject(value, indent + 1)+'\n';                
            } else {
                // If the value is not an object, append it to the result string
                result += value + '\n';
            }
            
        }
    }

    return result;
}
