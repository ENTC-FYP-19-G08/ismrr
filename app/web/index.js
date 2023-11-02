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
const dbRef = database.ref('/ISMRR/cmd/move'); // Replace with your database path

// Function to read the value from the database
dbRef.on('value', (snapshot) => {
    const value = snapshot.val();
    document.getElementById('dbValue').textContent = value;
});

// Function to update the value in the database
function sendTopic(topic) {    
    dbRef.child(topic).set(Date.now());    
}
