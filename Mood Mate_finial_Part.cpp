#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

bool parseInt(const string& text, int& value)
{
    stringstream ss(text);

    if (!(ss >> value))
        return false;

    ss >> ws;
    return ss.eof();
}

bool parseDouble(const string& text, double& value)
{
    stringstream ss(text);

    if (!(ss >> value))
        return false;

    ss >> ws;
    return ss.eof();
}

class MoodEntry
{
private:
    string date;
    int rating;
    string note;

public:
    MoodEntry();
    MoodEntry(string date, int rating, string note);

    void setDate(string date);
    void setRating(int rating);
    void setNote(string note);

    string getDate() const;
    int getRating() const;
    string getNote() const;
    string getMoodLabel() const;

    void printEntry() const;
};

MoodEntry::MoodEntry()
{
    date = "Unspecified";
    rating = 3;
    note = "No note";
}

MoodEntry::MoodEntry(string d, int r, string n)
{
    setDate(d);
    setRating(r);
    setNote(n);
}

void MoodEntry::setDate(string d)
{
    if (d != "")
        date = d;
    else
        date = "Unspecified";
}

void MoodEntry::setRating(int r)
{
    if (r >= 1 && r <= 5)
        rating = r;
    else
        rating = 3;
}

void MoodEntry::setNote(string n)
{
    if (n != "")
        note = n;
    else
        note = "No note";
}

string MoodEntry::getDate() const
{
    return date;
}

int MoodEntry::getRating() const
{
    return rating;
}

string MoodEntry::getNote() const
{
    return note;
}

string MoodEntry::getMoodLabel() const
{
    if (rating == 1)
        return "Very Bad";
    else if (rating == 2)
        return "Bad";
    else if (rating == 3)
        return "Okay";
    else if (rating == 4)
        return "Good";
    else
        return "Great";
}

void MoodEntry::printEntry() const
{
    cout << "Date: " << date << endl;
    cout << "Mood Rating: " << rating << endl;
    cout << "Mood: " << getMoodLabel() << endl;
    cout << "Note: " << note << endl;
}

class WellnessEntry : public MoodEntry
{
private:
    double sleepHours;
    int exerciseMinutes;

public:
    WellnessEntry();

    WellnessEntry(string date, int rating, string note,
                  double sleepHours, int exerciseMinutes);

    void setSleepHours(double hours);
    void setExerciseMinutes(int minutes);

    double getSleepHours() const;
    int getExerciseMinutes() const;

    void printEntry() const;
};

WellnessEntry::WellnessEntry()
    : MoodEntry()
{
    sleepHours = 0;
    exerciseMinutes = 0;
}

WellnessEntry::WellnessEntry(string d, int r, string n,
                             double s, int e)
    : MoodEntry(d, r, n)
{
    setSleepHours(s);
    setExerciseMinutes(e);
}

void WellnessEntry::setSleepHours(double hours)
{
    if (hours >= 0 && hours <= 24)
        sleepHours = hours;
    else
        sleepHours = 0;
}

void WellnessEntry::setExerciseMinutes(int minutes)
{
    if (minutes >= 0 && minutes <= 1440)
        exerciseMinutes = minutes;
    else
        exerciseMinutes = 0;
}

double WellnessEntry::getSleepHours() const
{
    return sleepHours;
}

int WellnessEntry::getExerciseMinutes() const
{
    return exerciseMinutes;
}

void WellnessEntry::printEntry() const
{
    MoodEntry::printEntry();
    cout << "Sleep Hours: " << sleepHours << endl;
    cout << "Exercise Minutes: " << exerciseMinutes << endl;
}

class WellnessLogger
{
private:
    WellnessEntry entries[20];
    int entryCount;

public:
    WellnessLogger();

    void addEntry(const WellnessEntry& entry);
    void displayEntries() const;

    void updateEntry(int index, string date, int rating,
                     string note, double sleepHours,
                     int exerciseMinutes);

    double calculateAverage() const;

    WellnessEntry getHighestMood() const;
    WellnessEntry getLowestMood() const;

    int getEntryCount() const;

    bool saveEntries() const;
    void loadEntries();
};

WellnessLogger::WellnessLogger()
{
    entryCount = 0;
}

void WellnessLogger::addEntry(const WellnessEntry& entry)
{
    if (entryCount < 20)
    {
        entries[entryCount] = entry;
        entryCount++;
    }
}

void WellnessLogger::displayEntries() const
{
    if (entryCount == 0)
    {
        cout << "No entries yet.\n";
        return;
    }

    for (int i = 0; i < entryCount; i++)
    {
        cout << "\nEntry " << i + 1 << endl;
        entries[i].printEntry();
    }
}

void WellnessLogger::updateEntry(int index,
                                 string date,
                                 int rating,
                                 string note,
                                 double sleepHours,
                                 int exerciseMinutes)
{
    if (index >= 0 && index < entryCount)
    {
        entries[index].setDate(date);
        entries[index].setRating(rating);
        entries[index].setNote(note);
        entries[index].setSleepHours(sleepHours);
        entries[index].setExerciseMinutes(exerciseMinutes);
    }
}

double WellnessLogger::calculateAverage() const
{
    if (entryCount == 0)
        return 0.0;

    int total = 0;

    for (int i = 0; i < entryCount; i++)
    {
        total += entries[i].getRating();
    }

    return static_cast<double>(total) / entryCount;
}

WellnessEntry WellnessLogger::getHighestMood() const
{
    WellnessEntry highest = entries[0];

    for (int i = 1; i < entryCount; i++)
    {
        if (entries[i].getRating() > highest.getRating())
        {
            highest = entries[i];
        }
    }

    return highest;
}

WellnessEntry WellnessLogger::getLowestMood() const
{
    WellnessEntry lowest = entries[0];

    for (int i = 1; i < entryCount; i++)
    {
        if (entries[i].getRating() < lowest.getRating())
        {
            lowest = entries[i];
        }
    }

    return lowest;
}

int WellnessLogger::getEntryCount() const
{
    return entryCount;
}

bool WellnessLogger::saveEntries() const
{
    ofstream outFile("moodmate.txt");

    if (!outFile)
    {
        cout << "Error: Could not save file.\n";
        return false;
    }

    for (int i = 0; i < entryCount; i++)
    {
        outFile << entries[i].getDate() << endl;
        outFile << entries[i].getRating() << endl;
        outFile << entries[i].getNote() << endl;
        outFile << entries[i].getSleepHours() << endl;
        outFile << entries[i].getExerciseMinutes() << endl;
    }

    if (!outFile)
    {
        cout << "Error: Could not write to file.\n";
        return false;
    }

    outFile.close();

    if (!outFile)
    {
        cout << "Error: Could not finish saving file.\n";
        return false;
    }

    cout << "Entries saved successfully.\n";
    return true;
}

void WellnessLogger::loadEntries()
{
    ifstream inFile("moodmate.txt");

    if (!inFile)
    {
        cout << "No saved file found. Starting with an empty log.\n";
        return;
    }

    entryCount = 0;

    string date;
    string ratingLine;
    string note;
    string sleepLine;
    string exerciseLine;

    int rating;
    double sleepHours;
    int exerciseMinutes;

    int recordNumber = 0;
    int extraRecords = 0;

    while (getline(inFile, date))
    {
        recordNumber++;

        if (!getline(inFile, ratingLine) ||
            !getline(inFile, note) ||
            !getline(inFile, sleepLine) ||
            !getline(inFile, exerciseLine))
        {
            cout << "Malformed record found at record "
                 << recordNumber
                 << ". Loading stopped.\n";
            break;
        }

        if (!parseInt(ratingLine, rating) ||
            !parseDouble(sleepLine, sleepHours) ||
            !parseInt(exerciseLine, exerciseMinutes))
        {
            cout << "Malformed numeric data found at record "
                 << recordNumber
                 << ". Loading stopped.\n";
            break;
        }

        WellnessEntry entry(date,
                            rating,
                            note,
                            sleepHours,
                            exerciseMinutes);

        if (entryCount < 20)
        {
            entries[entryCount] = entry;
            entryCount++;
        }
        else
        {
            extraRecords++;
        }
    }

    inFile.close();

    cout << entryCount << " entries loaded.\n";

    if (extraRecords > 0)
    {
        cout << extraRecords
             << " additional entries were not loaded "
             << "because the log limit is 20.\n";
    }
}

int main()
{
    WellnessLogger logger;
    logger.loadEntries();

    int choice;

    do
    {
        cout << "\n=========================\n";
        cout << "       MOODMATE\n";
        cout << "=========================\n";
        cout << "1. Add wellness entry\n";
        cout << "2. View entries\n";
        cout << "3. Update an entry\n";
        cout << "4. View average mood rating\n";
        cout << "5. Save entries\n";
        cout << "6. Save and exit\n";
        cout << "=========================\n";
        cout << "Enter choice: ";

        string choiceLine;
        getline(cin, choiceLine);

        if (!parseInt(choiceLine, choice))
        {
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1)
        {
            if (logger.getEntryCount() >= 20)
            {
                cout << "The log is full.\n";
                continue;
            }

            string date;
            string note;
            string input;

            int rating;
            double sleepHours;
            int exerciseMinutes;

            cout << "Enter date: ";
            getline(cin, date);

            cout << "Enter mood rating (1-5): ";
            getline(cin, input);

            if (!parseInt(input, rating))
            {
                cout << "Invalid rating input.\n";
                continue;
            }

            cout << "Enter sleep hours: ";
            getline(cin, input);

            if (!parseDouble(input, sleepHours))
            {
                cout << "Invalid sleep hours input.\n";
                continue;
            }

            cout << "Enter exercise minutes: ";
            getline(cin, input);

            if (!parseInt(input, exerciseMinutes))
            {
                cout << "Invalid exercise minutes input.\n";
                continue;
            }

            cout << "Enter note: ";
            getline(cin, note);

            WellnessEntry entry(date,
                                rating,
                                note,
                                sleepHours,
                                exerciseMinutes);

            logger.addEntry(entry);

            cout << "Entry added.\n";
        }

        else if (choice == 2)
        {
            logger.displayEntries();
        }

        else if (choice == 3)
        {
            if (logger.getEntryCount() == 0)
            {
                cout << "No entries to update.\n";
                continue;
            }

            logger.displayEntries();

            string input;
            int entryNumber;

            cout << "Enter entry number to update: ";
            getline(cin, input);

            if (!parseInt(input, entryNumber))
            {
                cout << "Invalid entry number.\n";
                continue;
            }

            if (entryNumber < 1 ||
                entryNumber > logger.getEntryCount())
            {
                cout << "Invalid entry number.\n";
                continue;
            }

            string date;
            string note;

            int rating;
            double sleepHours;
            int exerciseMinutes;

            cout << "Enter new date: ";
            getline(cin, date);

            cout << "Enter new mood rating (1-5): ";
            getline(cin, input);

            if (!parseInt(input, rating))
            {
                cout << "Invalid rating input.\n";
                continue;
            }

            cout << "Enter new sleep hours: ";
            getline(cin, input);

            if (!parseDouble(input, sleepHours))
            {
                cout << "Invalid sleep hours input.\n";
                continue;
            }

            cout << "Enter new exercise minutes: ";
            getline(cin, input);

            if (!parseInt(input, exerciseMinutes))
            {
                cout << "Invalid exercise minutes input.\n";
                continue;
            }

            cout << "Enter new note: ";
            getline(cin, note);

            logger.updateEntry(
                entryNumber - 1,
                date,
                rating,
                note,
                sleepHours,
                exerciseMinutes
            );

            cout << "Entry updated.\n";
        }

        else if (choice == 4)
        {
            if (logger.getEntryCount() == 0)
            {
                cout << "No entries to average.\n";
            }
            else
            {
                cout << fixed << setprecision(1);

                cout << "Average Mood: "
                     << logger.calculateAverage()
                     << endl;
            }
        }

        else if (choice == 5)
        {
            logger.saveEntries();
        }

        else if (choice == 6)
        {
            if (logger.saveEntries())
            {
                cout << "Entries saved. Goodbye!\n";
                break;
            }
            else
            {
                cout << "Save failed. Please try again.\n";
            }
        }

        else
        {
            cout << "Invalid menu choice.\n";
        }

    } while (true);

    return 0;
}