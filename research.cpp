#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <ctime>
#include <sstream>
#include <cctype>

/*
Name: research.cpp
Date created: 08/26/2026
Purpose: Gather research information for partcipants

*/


/*

Partcipants in study
saves to: partcipants.dat

*/

class Person {
private:
    int personID;
    std::string firstName;
    std::string lastName;
    int age;
    char gender;
    std::string treatment;
public:

    Person(int pID, std::string fName, std::string lName, int a, char g) :
    personID(pID), firstName(fName), lastName(lName), age(a), gender(g) {

        if (personID % 2 == 0) {
            treatment = "StudyMor";
        } else {
            treatment = "placebo";
        }

    }

    bool savePartcipant() {

        std::ofstream outFile("participants.dat", std::ios::app);
        if (!outFile) {
            return false;
        }
        
        outFile << personID << " " << quoted(firstName) << " " << quoted(lastName) 
            << " " << age << " " << gender << " " << quoted(treatment) << std::endl;

        return true;
    }

    int getPersonID() const {
        return personID;
    }

    std::string getFirstName() const {
        return firstName;
    }

    std::string getLastName() const {
        return lastName;
    }

    int getAge() const {
        return age;
    }

    char getGender() const {
        return gender;
    }

    std::string getTreatment() const {
        return treatment;
    }


};

/*

Survey Responses
Saves to: survey.dat

*/

class surveyResponses {

private:
    int personID;
    std::string surveyDate;
    bool headaches;
    bool constipation;
    bool sleepingTrouble;
    std::vector<std::string> sideEffects;
    bool studyMore; 

public:
    surveyResponses(
        int id,
        std::string date,
        bool h,
        bool c,
        bool s,
        std::vector<std::string> effects,
        bool more
    ) : personID(id), surveyDate(date), headaches(h), constipation(c), sleepingTrouble(s), sideEffects(effects), studyMore(more) {}

    bool saveSurvey() {
        std::ofstream outFile("survey.dat", std::ios::app);

        if (!outFile) {
            return false;
        }

        std::string effects;

        for (size_t i = 0; i < sideEffects.size(); i++) {
            if (i > 0) {
                effects += "; ";
            }
            effects += sideEffects[i];
        }

        outFile << std::quoted(surveyDate) << " "
                << personID << " "
                << headaches << " "
                << constipation << " "
                << sleepingTrouble << " "
                << std::quoted(effects) << " "
                << studyMore << '\n';

        return true;

    }        

};

void showMenu() {
    std::cout << "\n=========================\n";
    std::cout << "   Westlake Research Hospital\n";
    std::cout << "   StudyMor Study\n";
    std::cout << "=========================\n";
    std::cout << "1. Add New Participant\n";
    std::cout << "2. Collect Survey for Participant\n";
    std::cout << "3. Display Participants\n";
    std::cout << "4. Quit\n";
    std::cout << "=========================\n";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInteger(std::string prompt, int minimum) {
    int value;
    while (true) {
        std::cout << prompt;

        if (std::cin >> value && value >= minimum) {
            clearInput();
            return value;
        }

        std::cout << "Invalid input. Please input a number greater than or equal to " << minimum << std::endl;

        clearInput();

    }
}

int getNextID() {
    std::ifstream inFile("participants.dat");

    int id;
    int age;
    int highestID = 0;

    std::string firstName;
    std::string lastName;
    std::string treatment;
    char gender;

    while (inFile >> id
                  >> std::quoted(firstName)
                  >> std::quoted(lastName)
                  >> age
                  >> gender
                  >> std::quoted(treatment)) {

        if (id > highestID) {
            highestID = id;
        }
    }

    return highestID + 1;
}

std::vector<Person> loadParticipants() {
    std::vector<Person> partcipants;
    std::ifstream inFile("participants.dat");

    if (!inFile) {
        return partcipants;
    }

    int id;
    int age;
    std::string firstName;
    std::string lastName;
    std::string treatment;
    char gender;

    while (inFile >> id
                  >> std::quoted(firstName)
                  >> std::quoted(lastName)
                  >> age
                  >> gender
                  >> quoted(treatment)) {
        Person person(id, firstName, lastName, age, gender);
        partcipants.push_back(person);
    }

    return partcipants;
}

bool findParticipant(int id, Person& person) {
    std::vector<Person> participants = loadParticipants();

    for (const Person& p : participants) {
        if (p.getPersonID() == id) {
            person = p;
            return true;
        }
    }

    return false;
}

bool getYesNo(std::string question) {
    std::string answer;

    while (true) {
        std::cout << question << " (Y/N): ";
        std::getline(std::cin, answer);

        if (answer.empty()) {
            std::cout << "Please enter Y or N.\n";
            continue;
        }

        char choice = static_cast<char>(
            std::toupper(static_cast<unsigned char>(answer[0]))
        );

        if (choice == 'Y') {
            return true;
        }

        if (choice == 'N') {
            return false;
        }

        std::cout << "Invalid answer. Please enter Y or N.\n";
    }
}

std::string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    if (localTime == nullptr) {
        return "Unknown";
    }

    char date[11];

    strftime(date, sizeof(date), "%Y-%m-%d", localTime);

    return std::string(date);
}

// Option 1: Add New Participant

void addParticipant() {
    std::cout << "\n======== Add New Participant ========\n";

    std::string firstName;
    std::string lastName;
    int age;
    char gender;

    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);


    std::cout << "Enter last name: ";
    getline(std::cin, lastName);

    age = getInteger("Enter age: ", 0);

    std::cout << "Enter gender (M/F/O): ";

    while (true) {
        if (std::cin >> gender) {
            clearInput();
            gender = static_cast<char>(
                std::toupper(static_cast<unsigned char>(gender))
            );

            if (gender == 'M' || gender == 'F' || gender == 'O') {
                break;
            }
        } else {
            clearInput();
        }

        std::cout << "Please enter M, F, or O: ";
    }
    int id = getNextID();

    Person person(id, firstName, lastName, age, gender);

    if (person.savePartcipant()) {
        std::cout << "\nParticipant added!\n";
        std::cout << "Participant ID: " << person.getPersonID() << '\n';
        std::cout << "Name: " << person.getFirstName()
                  << " " << person.getLastName() << '\n';
        std::cout << "Treatment: " << person.getTreatment() << '\n';
    } else {
        std::cout << "\nError: Couldn't save participant.\n";
    }

}

// Option 2: Collect Survey

void collectSurvey() {
    std::vector<Person> partcipants = loadParticipants();
    if (partcipants.empty()) {
        std::cout << "\nNo participants found!" << std::endl;
        return;
    }
    std::cout << "\n========== Select a Participant ==========\n";

    std::cout << std::left
         << std::setw(8) << "ID"
         << std::setw(20) << "First Name"
         << std::setw(20) << "Last Name"
         << std::setw(15) << "Treatment" << '\n';

    std::cout << "---------------------------------------------------------------\n";

    for (const Person& p : partcipants) {
        std::cout << std::left
             << std::setw(8) << p.getPersonID()
             << std::setw(20) << p.getFirstName()
             << std::setw(20) << p.getLastName()
             << std::setw(15) << p.getTreatment() << '\n';
    }

    std::cout << '\n';

    int id = getInteger("Enter participant ID: ", 1);

    Person person(id, "", "", 0, 'O');

    if (!findParticipant(id, person)) {
        std::cout << "\nParticipant not found. Please try again.\n";
        return;
    }

    std::cout << "\n========== Weekly Survey ==========\n";
    std::cout << "Participant: "
         << person.getFirstName() << " "
         << person.getLastName() << '\n';

    std::cout << "Participant ID: " << person.getPersonID() << '\n';
    std::cout << "Assigned treatment: " << person.getTreatment() << '\n';
    std::cout << "Survey date: " << getCurrentDate() << "\n\n";

    std::string treatment = person.getTreatment();

    bool headaches = getYesNo(
        "Did you have any headaches while using " + treatment + "?"
    );

    bool constipation = getYesNo(
        "Did you have any constipation while using " + treatment + "?"
    );

    bool sleepingTrouble = getYesNo(
        "Did you experience any difficulty sleeping while using "
        + treatment + "?"
    );

    std::vector<std::string> sideEffects;

    std::string effect;

    std::cout << "\nList any other potential side effects you experienced.\n";
    std::cout << "Enter one side effect per line. Enter DONE when finished.\n";

    while (true) {
        std::cout << "Side effect: ";
        std::getline(std::cin, effect);

        if (effect == "DONE" || effect == "done" || effect == "Done") {
            break;
        }

        if (!effect.empty()) {
            sideEffects.push_back(effect);
        }
    }

    bool studyMore = getYesNo(
        "Did you feel like you could study more while using " + treatment + "?"
    );

    surveyResponses survey(
        person.getPersonID(),
        getCurrentDate(),
        headaches,
        constipation,
        sleepingTrouble,
        sideEffects,
        studyMore
    );

    if (survey.saveSurvey()) {
        std::cout << "\nSurvey saved successfully!\n";
    }
    else {
        std::cout << "\nError: Could not save survey information.\n";
    }
}

// Option 3: Display Participants

void displayParticipants() {
    std::vector<Person> participants = loadParticipants();

    std::cout << "\n========== Study Participants ==========\n";

    if (participants.empty()) {
        std::cout << "No participants found.\n";
        return;
    }

    std::cout << std::left
         << std::setw(8) << "ID"
         << std::setw(18) << "First Name"
         << std::setw(18) << "Last Name"
         << std::setw(8) << "Age"
         << std::setw(10) << "Gender"
         << std::setw(15) << "Treatment" << '\n';

    std::cout << "--------------------------------------------------------------------------\n";

    for (const Person& p : participants) {
        std::cout << std::left
             << std::setw(8) << p.getPersonID()
             << std::setw(18) << p.getFirstName()
             << std::setw(18) << p.getLastName()
             << std::setw(8) << p.getAge()
             << std::setw(10) << p.getGender()
             << std::setw(15) << p.getTreatment() << '\n';
    }

    std::cout << "\nTotal participants: " << participants.size() << '\n';
}

int main() {
    int choice;
    do {
        showMenu();
        choice = getInteger("Enter your choice: ", 1);

        switch (choice) {
            case 1:
                addParticipant();
                break;
            case 2:
                collectSurvey();
                break;
            case 3:
                displayParticipants();
                break;
            case 4:
                std::cout << "\nExiting Program!\n";
                break;
            default:
                std::cout << "\nInvalid choice. Please select 1, 2, 3, or 4\n";
        }

    } while (choice != 4);
    return 0;
}