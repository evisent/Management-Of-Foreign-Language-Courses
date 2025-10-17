let stateData = {};

async function createStudent() {
    try {
        addToOutput("Creating new student...");
        const response = await fetch('/create_student', {
            method: 'POST'
        });
        const data = await response.json();
        addToOutput("✅ Student created successfully!");
        getState();
    } catch (error) {
        addToOutput("❌ Error creating student: " + error);
    }
}

async function getState() {
    try {
        const response = await fetch('/state');
        stateData = await response.json();
        updateUI();
        addToOutput("System state refreshed.");
    } catch (error) {
        addToOutput("❌ Error fetching state: " + error);
    }
}

async function getGroups() {
    try {
        addToOutput("Fetching groups data...");
        const response = await fetch('/groups');
        const data = await response.json();
        
        displayGroups(data.groups);
        addToOutput("Groups data loaded.");
    } catch (error) {
        addToOutput("❌ Error fetching groups: " + error);
    }
}

async function resetSchool() {
    try {
        addToOutput("Resetting school...");
        const response = await fetch('/reset', {
            method: 'POST'
        });
        const data = await response.json();
        addToOutput("✅ School reset successfully!");
        getState();
        document.getElementById('groupsContainer').style.display = 'none';
    } catch (error) {
        addToOutput("❌ Error resetting school: " + error);
    }
}

function updateUI() {
    document.getElementById('studentsCount').textContent = stateData.students_count || 0;
    document.getElementById('groupsCount').textContent = stateData.groups_count || 0;
    document.getElementById('lastAction').textContent = stateData.last_action || 'No actions yet';
    document.getElementById('status').textContent = stateData.status || 'UNKNOWN';
}

function displayGroups(groups) {
    const container = document.getElementById('groupsContainer');
    const list = document.getElementById('groupsList');
    
    if (!groups || groups.length === 0) {
        list.innerHTML = '<div class="group-card">No groups available. Create some students first!</div>';
    } else {
        list.innerHTML = groups.map(group => `
            <div class="group-card">
                <div class="group-header">
                    <strong>${group.language || 'Unknown'}</strong>
                    <span>Level ${group.level || 1}</span>
                </div>
                <div>Intensity: ${group.intensity || 1}</div>
                <div style="margin-top: 10px;">
                    ${(group.students || []).map(student => 
                        `<div class="student-item">${student}</div>`
                    ).join('')}
                </div>
            </div>
        `).join('');
    }
    
    container.style.display = 'block';
}

function addToOutput(message) {
    const output = document.getElementById('output');
    const timestamp = new Date().toLocaleTimeString();
    output.textContent += `\n[${timestamp}] ${message}`;
    output.scrollTop = output.scrollHeight;
}

// Initialize
addToOutput("Language School System Started");
getState();