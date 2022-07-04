import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View, SafeAreaView, TextInput, TouchableOpacity,Pressable} from 'react-native';
import React, {useState, useEffect} from 'react';

import Ionicons from '@expo/vector-icons/Ionicons';
import { FontAwesome } from '@expo/vector-icons';
import { MaterialCommunityIcons } from '@expo/vector-icons';
import { MaterialIcons } from '@expo/vector-icons';
import BasicSpeedometer from 'react-native-speedometer-chart';
import { Line, Text as SvgText, G } from 'react-native-svg'
import { Button } from 'react-native-paper';
// import RNSpeedometer from 'react-native-speedometer'
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
import {collection, getFirestore, onSnapshot, doc, setDoc}  from "firebase/firestore";
import Speedometer , {
  Background,
  Arc,
  Needle,
  Progress,
  Marks,
  Indicator,
  DangerPath,
} from 'react-native-cool-speedometer';
import guestScreen from './guestScreen';


const firebaseConfig = {
  apiKey: "AIzaSyCqnxinzpFRFkfIIShDvlw8uVr_ULMm2CA",
  authDomain: "percy-dashboard.firebaseapp.com",
  projectId: "percy-dashboard",
  storageBucket: "percy-dashboard.appspot.com",
  messagingSenderId: "396244618046",
  appId: "1:396244618046:web:61eecd0942620697c8fa6f",
  measurementId: "G-ETGNWVXGB0"
};

const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const db = getFirestore(app);
const commandRef = doc(db, "Commands", "PercyCommands");
const deltaRef = doc(db, "Commands", "DeltaCommands");
export default function loginScreen({navigation}) {





const [userName, setUserName] = useState(null);
const [password, setPassword] = useState(null);
const [authSucessful, setAuthSucessful] = useState(true)
const adminUsername = "admin";
const adminPassword = "PercyIsTheBest";
 

  
const auth = ()=>{
  if(userName == adminUsername && password==adminPassword){
console.log("Login sucessful");
setAuthSucessful(true) 
navigation.navigate("adminScreen")
  }else{
    console.log("Incorrect password")
    setAuthSucessful(false)
  }

}


 function RenderIncorrectDetails(){
if(authSucessful==false){

  console.log("render error text")
 return (<Text style={{paddingBottom:20, color:"red"}}> Incorrect username or password</Text>)

}else{
   return (<View></View>)
}
 }

  return (
    <SafeAreaView style={{flex:1}}>
<View style={styles.topBar}>
  <View style={{flexDirection:"row", alignItems:"center",flexWrap:"wrap"}}>
    <View style={{flex:1, }}>

{/* Border */}
  <Text style={{color:"#fff", fontSize:70, fontWeight:'500', }}>
    Percy Dashboard
  </Text>
    </View>

          <View style={{flex:1, alignItems:"flex-end", flexWrap:"wrap"}}>
            <View style={{flex:1, flexDirection:"row",paddingEnd:40}}> 
            <View style={{alignItems:"center", paddingEnd:50}}>

            
            <View style={{flexDirection:"row"}}> 
            
            </View>
            </View>
            <View style={{alignItems:"center"}}>

           
            <View style={{flexDirection:"row", }}> 
            
                    </View>
            </View>
            </View>
          </View>
     </View>
  
  </View>
<View style={{flex:5, backgroundColor:"white",justifyContent:"center", alignItems:"center"}}>
<Text style={{fontSize:40, fontWeight:"100", paddingBottom:50}}> Welcome to the Percy Dashboard {"\n"} please login or continue as a guest</Text>
<Text>Username</Text>
<TextInput
        style={styles.input}
        onChangeText={setUserName}
        value={userName}
        placeholder="Username"

      />
      <Text>Password</Text>
      <TextInput
        style={styles.input}
        onChangeText={setPassword}
        value={password}
        placeholder="Password"
        keyboardType="numeric"
        secureTextEntry={true}

      />
  
    <RenderIncorrectDetails/>

<Button mode="contained" onPress={auth}  color='#19c416'
              
              contentStyle={{height:50,width:"100%",justifyContent: 'center', alignContent:"center",
             }} style={{width:150, height:50}}>
            Sign in
            </Button>
            <Button mode="contained" onPress={() => {navigation.navigate(guestScreen)}}  color='#19c416'
              
              contentStyle={{height:50,width:"100%",justifyContent: 'center', alignContent:"center",
             }} style={{width:300, height:50, marginTop:10}}>
            continue as guest
            </Button>
             </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {


flex: 1,
  },
  topBar: {
  flex:1,
  flexWrap:"wrap",
  backgroundColor: '#19c416',
  justifyContent: 'center',
    
  },
  bottom:{
flex:3,
backgroundColor: '#fff',
alignItems: 'center',
justifyContent: 'center',
  },
  input: {
    height: 40,
    width:250,
    margin: 12,
    borderWidth: 1,
    padding: 10,
  },
  
});
