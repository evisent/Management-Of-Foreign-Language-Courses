let stateData = {};
let languagesData = {};

async function createStudent() {
    try {
        const response = await fetch('/create_student', {
            method: 'POST'
        });
        const data = await response.json();
        getState();
        addToOutput("✅ Студент успешно создан!");
        loadLanguages(); // Перезагружаем языки после создания студента
    } catch (error) {
        addToOutput("❌ Ошибка при создании студента: " + error);
    }
}

async function deleteStudent() {
    try{
        const response = await fetch('/delete_student', {
            method: 'POST'
        })
        const data = await response.json();
        addToOutput("✅ Студент покинул курс");
        loadLanguages();
        getState();
    } catch(error){
        addToOutput("❌ Ошибка при удалении студента: " + error);
    }
}

async function getState() {
    try {
        const response = await fetch('/state');
        stateData = await response.json();
        updateUI();
    } catch (error) {
        addToOutput("❌ Ошибка при получении состояния: " + error);
    }
}

async function loadLanguages() {
    try {
        const response = await fetch('/groups');
        const data = await response.json();
        languagesData = data.groups || [];
        displayLanguages(languagesData);
    } catch (error) {
        addToOutput("❌ Ошибка при загрузке языков: " + error);
    }
}

function displayLanguages(groups) {
    const list = document.getElementById('languagesList');
    
    if (!groups || groups.length === 0) {
        list.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">🌍</div>
                <h3>Группы не найдены</h3>
                <p>Создайте студентов, чтобы сформировать группы</p>
            </div>
        `;
        return;
    }

    // Группируем по языкам
    const languagesMap = {};
    groups.forEach(group => {
        const lang = group.language || 'Unknown';
        if (!languagesMap[lang]) {
            languagesMap[lang] = [];
        }
        languagesMap[lang].push(group);
    });

    list.innerHTML = Object.entries(languagesMap).map(([language, langGroups]) => {
        const totalStudents = langGroups.reduce((sum, group) => sum + (group.students?.length || 0), 0);
        const totalRevenue = langGroups.reduce((sum, group) => {
            const groupRevenue = (group.students || []).reduce((studentSum, student) => 
                studentSum + (student.price || 0), 0
            );
            return sum + groupRevenue;
        }, 0);
        
        return `
            <div class="language-item">
                <div class="language-header" onclick="toggleLanguage('${language}')">
                    <div class="language-name">
                        ${getLanguageFlag(language)} ${language}
                        <span style="font-size: 0.8em; color: var(--success); margin-left: 8px;">
                            💰 ${totalRevenue} ₽
                        </span>
                    </div>
                    <div style="display: flex; align-items: center; gap: 10px;">
                        <span class="language-count">${langGroups.length} групп</span>
                        <span class="language-toggle">▼</span>
                    </div>
                </div>
                <div class="groups-list" id="groups-${language}">
                    ${langGroups.map(group => {
                        const groupRevenue = (group.students || []).reduce((sum, student) => 
                            sum + (student.price || 0), 0
                        );
                        const avgPrice = group.students?.length ? 
                            Math.round(groupRevenue / group.students.length) : 0;
                            
                        return `
                        <div class="group-item" onclick="showGroupDetails('${language}', ${group.level}, '${group.intensity_name}')">
                            <div class="group-info">
                                <div>
                                    <div class="group-level">Уровень ${group.level || 1}</div>
                                    <div class="group-intensity">
                                        ${group.intensity_name || 'Unknown'} (${group.intensity_days || 1} дней)
                                        <div style="color: var(--success); font-size: 0.8em; margin-top: 2px;">
                                            💰 ${avgPrice} ₽/студ
                                        </div>
                                    </div>
                                </div>
                                <div class="group-students-count">${group.students?.length || 0}</div>
                            </div>
                        </div>
                    `}).join('')}
                </div>
            </div>
        `;
    }).join('');
}

function getLanguageFlag(language) {
    const flags = {
        'English': 'EN',
        'Spanish': 'ES', 
        'French': 'FR',
        'German': 'DE',
        'Chinese': 'CN',
    };
    return flags[language] || '🌍';
}

function toggleLanguage(language) {
    const groupsList = document.getElementById(`groups-${language}`);
    const toggle = groupsList.previousElementSibling.querySelector('.language-toggle');
    
    groupsList.classList.toggle('expanded');
    toggle.classList.toggle('expanded');
}

function showGroupDetails(language, level, intensityName) {
    // Находим группу в данных
    const group = languagesData.find(g => 
        g.language === language && 
        g.level === level && 
        g.intensity_name === intensityName
    );

    if (!group) return;

    // Убираем выделение с других групп
    document.querySelectorAll('.group-item').forEach(item => {
        item.classList.remove('active');
    });

    // Добавляем выделение текущей группе
    event.target.closest('.group-item').classList.add('active');

    // Показываем детали
    const details = document.getElementById('groupDetails');
    details.innerHTML = `
        <div class="details-card">
            <div class="details-header">
                <div class="details-title">
                    ${getLanguageFlag(language)} ${language} - Уровень ${level}
                </div>
            </div>
            
            <div class="details-meta">
                <div class="meta-item">
                    <div class="meta-label">Интенсивность</div>
                    <div class="meta-value">${intensityName} (${group.intensity_days || 1} дней)</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Количество студентов</div>
                    <div class="meta-value">${group.students?.length || 0}</div>
                </div>
                <div class="meta-item">
                    <div class="meta-label">Средняя цена</div>
                    <div class="meta-value">${calculateAveragePrice(group.students)} ₽</div>
                </div>
            </div>

            <div class="students-section">
                <h4>👥 Студенты в группе</h4>
                <div class="students-grid">
                    ${(group.students || []).map(student => `
                        <div class="student-card">
                            <div class="student-avatar">
                                ${student.name ? student.name.charAt(0).toUpperCase() : '?'}
                            </div>
                            <div class="student-info">
                                <div class="student-name">${student.name || 'Неизвестный'}</div>
                                <div class="student-languages">
                                    <span>Цена: ${student.price || 0} ₽</span>
                                    <span style="margin-left: 10px; color: var(--gray);">${language}</span>
                                </div>
                            </div>
                        </div>
                    `).join('')}
                </div>
            </div>
        </div>
    `;

    addToOutput(`📖 Открыта группа: ${language} - Уровень ${level}`);
}

// Добавьте эту вспомогательную функцию для расчета средней цены
function calculateAveragePrice(students) {
    if (!students || students.length === 0) return 0;
    
    const total = students.reduce((sum, student) => sum + (student.price || 0), 0);
    return Math.round(total / students.length);
}

function clearSelection() {
    document.querySelectorAll('.group-item').forEach(item => {
        item.classList.remove('active');
    });
    document.getElementById('groupDetails').innerHTML = `
        <div class="empty-state">
            <div class="empty-icon">👆</div>
            <h3>Выберите группу</h3>
            <p>Выберите язык или группу из списка слева чтобы увидеть детали</p>
        </div>
    `;
}

async function resetSchool() {
    if (!confirm("Вы уверены, что хотите очистить все данные? Это действие нельзя отменить.")) {
        return;
    }
    
    try {
        const response = await fetch('/reset', {
            method: 'POST'
        });
        const data = await response.json();
        addToOutput("✅ Система успешно сброшена!");
        getState();
        clearSelection();
        loadLanguages();
    } catch (error) {
        addToOutput("❌ Ошибка при сбросе системы: " + error);
    }
}



function updateUI() {
    document.getElementById('studentsCount').textContent = stateData.students_count || 0;
    document.getElementById('groupsCount').textContent = stateData.groups_count || 0;
}

function clearOutput() {
    document.getElementById('output').textContent = "Журнал очищен\n";
    addToOutput("🗑️ Журнал событий очищен");
}

function addToOutput(message) {
    const output = document.getElementById('output');
    const timestamp = new Date().toLocaleTimeString();
    output.textContent += `[${timestamp}] ${message}\n`;
    output.scrollTop = output.scrollHeight;
}

// Initialize
document.addEventListener('DOMContentLoaded', function() {
    addToOutput("🚀 Система языковой школы запущена");
    getState();
    loadLanguages();
});