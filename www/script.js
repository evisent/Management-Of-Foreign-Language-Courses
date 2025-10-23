let stateData = {};
let languagesData = {};

const IS_INITIALIZED_KEY = 'languageSchoolInitialized';
const LANGUAGES = ['English', 'Spanish', 'German', 'Chinese'];
const LEVELS = [1, 2, 3];
const INTENSITIES = [
    { name: 'Поддерживающее обучение', periods: 3 },      // Поддерживающее обучение
    { name: 'Обычный', periods: 4 },   // Обычный
    { name: 'интенсив', periods: 6 }   // интенсив
];

let selectedOption = null;

document.addEventListener('DOMContentLoaded', function() {
    const isInitialized = localStorage.getItem(IS_INITIALIZED_KEY);
    
    if (!isInitialized) {
        showInitModal();
    } else {
        closeInitModal();
    }
    
    getState();
    loadLanguages();
});

function showInitModal() {
    document.getElementById('initModal').style.display = 'flex';
}

function closeInitModal() {
    document.getElementById('initModal').style.display = 'none';
}

function selectOption(option) {
    selectedOption = option;
    
    // Убираем выделение со всех карточек
    document.querySelectorAll('.option-card').forEach(card => {
        card.classList.remove('selected');
    });
    
    // Добавляем выделение выбранной карточке
    event.currentTarget.classList.add('selected');
    
    if (option === 'auto') {
        createRandomStudents();
    } else if (option === 'manual') {
        showManualForm();
    }
}

function showManualForm() {
    document.querySelector('.init-options').style.display = 'none';
    document.getElementById('manualForm').style.display = 'block';
    generateManualStudentsForm();
}

function backToOptions() {
    document.getElementById('manualForm').style.display = 'none';
    document.querySelector('.init-options').style.display = 'grid';
    selectedOption = null;
    
    document.querySelectorAll('.option-card').forEach(card => {
        card.classList.remove('selected');
    });
}

function generateManualStudentsForm() {
    const grid = document.getElementById('manualStudentsGrid');
    grid.innerHTML = '';
    
    for (let i = 1; i <= 15; i++) {
        const studentCard = `
            <div class="student-form-card" data-student-id="${i}">
                <div class="student-header">
                    <div class="student-avatar-form">${i}</div>
                    <input type="text" class="student-name-input" placeholder="Имя студента" value="Студент ${i}">
                </div>
                
                <div class="languages-container" id="languages-${i}">
                    <div class="language-row">
                        <select class="form-select student-language">
                            <option value="">Выберите язык</option>
                            ${LANGUAGES.map(lang => `<option value="${lang}">${lang}</option>`).join('')}
                        </select>
                        <select class="form-select student-level">
                            <option value="">Уровень</option>
                            ${LEVELS.map(level => `<option value="${level}">${level}</option>`).join('')}
                        </select>
                        <select class="form-select student-intensity">
                            <option value="">Интенсивность</option>
                            ${INTENSITIES.map(intensity => 
                                `<option value="${intensity.name}">${intensity.name} (${intensity.periods} периодов)</option>`
                            ).join('')}
                        </select>
                        <button class="remove-language" onclick="removeLanguage(${i}, this)" ${i === 1 ? 'style="visibility:hidden;"' : ''}>×</button>
                    </div>
                </div>
                
                <button class="add-language" onclick="addLanguage(${i})">
                    + Добавить язык
                </button>
            </div>
        `;
        grid.innerHTML += studentCard;
    }
}

function addLanguage(studentId) {
    const container = document.getElementById(`languages-${studentId}`);
    const languageCount = container.querySelectorAll('.language-row').length;
    
    const languageRow = document.createElement('div');
    languageRow.className = 'language-row';
    languageRow.innerHTML = `
        <select class="form-select student-language">
            <option value="">Выберите язык</option>
            ${LANGUAGES.map(lang => `<option value="${lang}">${lang}</option>`).join('')}
        </select>
        <select class="form-select student-level">
            <option value="">Уровень</option>
            ${LEVELS.map(level => `<option value="${level}">${level}</option>`).join('')}
        </select>
        <select class="form-select student-intensity">
            <option value="">Интенсивность</option>
            ${INTENSITIES.map(intensity => 
                `<option value="${intensity.name}">${intensity.name} (${intensity.periods} периодов)</option>`
            ).join('')}
        </select>
        <button class="remove-language" onclick="removeLanguage(${studentId}, this)">×</button>
    `;
    
    container.appendChild(languageRow);
}

function removeLanguage(studentId, button) {
    const container = document.getElementById(`languages-${studentId}`);
    const rows = container.querySelectorAll('.language-row');
    
    if (rows.length > 1) {
        button.parentElement.remove();
    }
}

async function createRandomStudents() {
    try {
        addToOutput("🎲 Создание 15 случайных студентов...");
        
        const response = await fetch('/create_random_students', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            }
        });
        
        if (!response.ok) throw new Error('Ошибка сервера');
        
        const data = await response.json();
        addToOutput("✅ 15 случайных студентов созданы!");
        
        // Сохраняем состояние инициализации
        localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        closeInitModal();
        loadLanguages();
        getState();
        
    } catch (error) {
        addToOutput("❌ Ошибка при создании случайных студентов: " + error);
    }
}

async function createManualStudents() {
    const studentCards = document.querySelectorAll('.student-form-card');
    const studentsData = [];
    
    // Собираем данные студентов (остальной код без изменений)
    for (let i = 0; i < studentCards.length; i++) {
        const card = studentCards[i];
        const name = card.querySelector('.student-name-input').value || `Студент ${i + 1}`;
        const languageRows = card.querySelectorAll('.language-row');
        
        const languages = [];
        let hasEmptyFields = false;
        
        for (const row of languageRows) {
            const language = row.querySelector('.student-language').value;
            const level = row.querySelector('.student-level').value;
            const intensity = row.querySelector('.student-intensity').value;
            
            if (!language || !level || !intensity) {
                hasEmptyFields = true;
                break;
            }
            
            languages.push({
                language: language,
                level: level,
                intensity: intensity
            });
        }
        
        if (hasEmptyFields) {
            alert(`Пожалуйста, заполните все поля для студента ${i + 1}`);
            return;
        }
        
        if (languages.length === 0) {
            alert(`Студент ${i + 1} должен изучать хотя бы один язык`);
            return;
        }
        
        studentsData.push({
            name: name,
            languages: languages
        });
    }
    
    try {
        addToOutput("🔄 Создание студентов...");
        
        const response = await fetch('/create_manual_students', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(studentsData)
        });
        
        if (!response.ok) throw new Error('Ошибка сервера');
        
        const data = await response.json();
        addToOutput(`✅ ${data.count} студентов успешно создано!`);
        
        // Сохраняем состояние инициализации
        localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        closeInitModal();
        loadLanguages();
        getState();
        
    } catch (error) {
        addToOutput("❌ Ошибка при создании студентов: " + error);
    }
}

let isCreating = false;
async function createStudent() {
    if(isCreating){
        addToOutput("⏳ Подождите, создание уже выполняется...");
        return;
    }

    isCreating = true;
    const button = event.target;
    const originalText = button.innerHTML;
    
    button.innerHTML = '⏳ Создание...';
    button.disabled = true;

    try {
        const response = await fetch('/step', {
            method: 'POST'
        });
        const data = await response.json();
        getState();
        addToOutput("✅ Студент успешно создан!");
        loadLanguages();
        
        if (!localStorage.getItem(IS_INITIALIZED_KEY)) {
            localStorage.setItem(IS_INITIALIZED_KEY, 'true');
        }
        
    } catch (error) {
        addToOutput("❌ Ошибка при создании студента: " + error);
    } finally{
        button.innerHTML = originalText;
        button.disabled = false;
        isCreating = false;
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
        console.log("🔄 Starting loadLanguages...");
        
        const response = await fetch('/groups');
        console.log("📡 Response status:", response.status, response.statusText);
        
        if (!response.ok) {
            throw new Error(`HTTP ${response.status}: ${response.statusText}`);
        }
        
        // Сначала получаем текст для диагностики
        const text = await response.text();
        console.log("📄 Raw response text:", text);
        console.log("📏 Response length:", text.length);
        
        if (!text || text.trim() === '') {
            throw new Error('Server returned empty response');
        }
        
        // Пытаемся парсить JSON
        let data;
        try {
            data = JSON.parse(text);
        } catch (jsonError) {
            console.error("❌ JSON parse error:", jsonError);
            console.error("💥 Problematic JSON:", text.substring(0, 200) + "...");
            throw new Error(`Invalid JSON from server: ${jsonError.message}`);
        }
        
        console.log("✅ JSON parsed successfully, groups count:", data.groups?.length || 0);
        
        languagesData = data.groups || [];
        displayLanguages(languagesData);
        
    } catch (error) {
        console.error("💥 loadLanguages error:", error);
        addToOutput("❌ Ошибка при загрузке языков: " + error.message);
        
        // Показываем детальную ошибку в интерфейсе
        const list = document.getElementById('languagesList');
        list.innerHTML = `
            <div class="empty-state error-state">
                <div class="empty-icon">❌</div>
                <h3>Ошибка загрузки данных</h3>
                <p>${error.message}</p>
                <button onclick="loadLanguages()" class="retry-btn">Повторить загрузку</button>
            </div>
        `;
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
                                        ${group.intensity_name || 'Unknown'} (${group.periods_left || 1} периодов осталось)
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
        'Arabian': 'AR'
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
    const group = languagesData.find(g => 
        g.language === language && 
        g.level === level && 
        g.intensity_name === intensityName
    );

    if (!group) return;

    document.querySelectorAll('.group-item').forEach(item => {
        item.classList.remove('active');
    });

    event.target.closest('.group-item').classList.add('active');

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
                    <div class="meta-value">${intensityName} (${group.periods_left || 1} периодов всего)</div>
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
                                    <span style="margin-left: 10px; color: var(--success);">
                                        ${student.student_periods_left || 0} периодов осталось
                                    </span>
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

function openTab(tabName) {
    // Скрыть все вкладки
    document.querySelectorAll('.tab-content').forEach(tab => {
        tab.classList.remove('active');
    });
    
    // Убрать активность со всех кнопок
    document.querySelectorAll('.tab-button').forEach(button => {
        button.classList.remove('active');
    });
    
    // Показать выбранную вкладку
    document.getElementById(tabName + '-tab').classList.add('active');
    event.currentTarget.classList.add('active');
    
    // Загрузить данные если нужно
    if (tabName === 'individual') {
        loadIndividualStudents();
    }
}

// Загрузка индивидуальных студентов
async function loadIndividualStudents() {
    try {
        const response = await fetch('/individual_students');
        const data = await response.json();
        displayIndividualStudents(data.individual_students || []);
        
    } catch (error) {
        console.error('Error loading individual students:', error);
        document.getElementById('individualList').innerHTML = `
            <div class="empty-state error-state">
                <div class="empty-icon">❌</div>
                <h3>Ошибка загрузки</h3>
                <p>${error.message}</p>
            </div>
        `;
    }
}

function displayIndividualStudents(students) {
    const list = document.getElementById('individualList');
    
    if (!students || students.length === 0) {
        list.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">🎯</div>
                <h3>Нет индивидуальных студентов</h3>
                <p>Студенты появятся здесь, когда их группы станут меньше 5 человек</p>
            </div>
        `;
        return;
    }
    
    list.innerHTML = students.map(student => {
        const totalRevenue = student.languages?.reduce((sum, lang) => sum + (lang.price || 0), 0) || 0;
        
        return `
            <div class="individual-student-card">
                <div class="student-header-individual">
                    <div class="student-avatar-individual">
                        ${student.name ? student.name.charAt(0).toUpperCase() : '?'}
                    </div>
                    <div>
                        <div class="student-name-individual">${student.name || 'Неизвестный'}</div>
                        <div class="student-revenue">💰 ${totalRevenue} ₽/период</div>
                    </div>
                </div>
                
                <div class="student-languages-individual">
                    <strong>Изучаемые языки:</strong>
                    ${(student.languages || []).map(lang => {
                        const periodsLeft = lang.periods_left || 0;
                        const totalPeriods = lang.total_periods || 0;
                        const intensityName = getIntensityName(totalPeriods);
                        
                        return `
                        <div class="language-tag">
                            ${lang.name || 'Unknown'} 
                            (Ур. ${lang.level || 1}) 
                            - ${periodsLeft} из ${totalPeriods} периодов
                            <br>
                            <small>${intensityName} • ${lang.price || 0} ₽</small>
                        </div>
                    `}).join('')}
                </div>
            </div>
        `;
    }).join('');
}

// Функция для получения названия интенсивности
function getIntensityName(totalPeriods) {
    switch(totalPeriods) {
        case 3: return 'Интенсив';
        case 4: return 'Обычный';
        case 6: return 'Поддерживающее обучение';
        default: return `${totalPeriods} периодов`;
    }
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
        
        // Удаляем состояние инициализации при сбросе
        localStorage.removeItem(IS_INITIALIZED_KEY);
        
        getState();
        clearSelection();
        loadLanguages();
        showInitModal(); // Показываем окно инициализации после сброса
        
    } catch (error) {
        addToOutput("❌ Ошибка при сбросе системы: " + error);
    }
}

function updateUI() {
    document.getElementById('studentsCount').textContent = stateData.students_count || 0;
    document.getElementById('groupsCount').textContent = stateData.groups_count || 0;
    document.getElementById('individualCount').textContent = stateData.individual_count || 0;
    document.getElementById('profit').textContent = (stateData.profit || 0) + ' ₽';
    document.getElementById('total').textContent = (stateData.total || 0) + ' ₽';
    document.getElementById('curPeriod').textContent = (stateData.cur_period || 0);

    if (document.getElementById('individual-tab').classList.contains('active')) {
        loadIndividualStudents();
    }
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

